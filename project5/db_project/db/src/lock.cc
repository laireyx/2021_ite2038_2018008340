/**
 * @addtogroup LockManager
 * @{
 */
#include <const.h>
#include <lock.h>
#include <transaction.h>

#include <iostream>
#include <new>
#include <unordered_map>

/// @brief Lock manager mutex.
pthread_mutex_t* lock_manager_mutex;

/** @todo change it to MAX_TABLE_INSTANCE */
std::unordered_map<LockLocation, LockList*> lock_instances;
/// @brief Transaction waiting list(Deadlock waiting graph).
std::unordered_map<trxid_t, std::unordered_set<trxid_t>> trx_wait;

namespace lock_helper {

bool _find_deadlock(trxid_t current, trxid_t root, std::unordered_set<trxid_t> visit) {
    if (visit.find(current) != visit.end()) return false;
    visit.insert(current);
    if (current == 0) {
        return false;
    }
    if (current == root) {
        return true;
    }
    for (auto& occupant : trx_wait[current]) {
        if (_find_deadlock(occupant, root, visit)) {
            return true;
        }
    }
    return false;
}
bool find_deadlock(trxid_t root) {
    std::unordered_set<trxid_t> visit;
    for (auto& occupant : trx_wait[root]) {
        if (_find_deadlock(occupant, root, visit)) {
            return true;
        }
    }
    return false;
}

}  // namespace lock_helper

int init_lock_table() {
    try {
        lock_manager_mutex = new pthread_mutex_t;
        if (pthread_mutex_init(lock_manager_mutex, nullptr)) {
            return -1;
        }
        lock_instances.clear();
        trx_wait.clear();
    } catch (std::bad_alloc exception) {
        return -1;
    }
    return 0;
}

int cleanup_lock_table() {
    pthread_mutex_destroy(lock_manager_mutex);
    delete lock_manager_mutex;
    lock_manager_mutex = nullptr;
    lock_instances.clear();
    return 0;
}

Lock* lock_acquire(int table_id, pagenum_t page_id, int key_idx,
                   trxid_t trx_id, int lock_mode) {
    if(key_idx < 0 || key_idx >= 64) {
        return nullptr;
    }
    pthread_mutex_lock(lock_manager_mutex);
    Lock* lock_instance = new Lock;
    LockLocation lock_location = std::make_pair(table_id, page_id);

    lock_instance->lock_mode = static_cast<LockMode>(lock_mode);
    lock_instance->acquired = false;

    lock_instance->cond = new pthread_cond_t;
    pthread_cond_init(lock_instance->cond, nullptr);

    lock_instance->trx_id = trx_id;
    lock_instance->mask = 0;
    lock_helper::set_bit(lock_instance, key_idx);

    if (lock_instances.find(lock_location) == lock_instances.end()) {
        LockList* new_lock_list = new LockList;
        new_lock_list->lock_location = lock_location;
        new_lock_list->head = new_lock_list->tail = lock_instance;

        lock_instance->acquired = true;
        lock_instance->list = new_lock_list;
        lock_instance->prev = nullptr;
        lock_instance->next = nullptr;
        lock_instance->next_trx = nullptr;

        lock_instances[lock_location] = new_lock_list;
        pthread_mutex_unlock(lock_manager_mutex);
        return lock_instance;
    }

    lock_instance->list = lock_instances[lock_location];

    Lock* existing_lock = lock_instances[lock_location]->tail;

    // Check if this lock is waiting consecutive slocks.
    bool consecutive_slocks = false;

    if (trx_wait.find(trx_id) == trx_wait.end()) {
        trx_wait[trx_id] = std::unordered_set<trxid_t>();
    }

    while(existing_lock) {
        if (lock_helper::get_bit(existing_lock, key_idx) &&
            existing_lock->trx_id != trx_id
        ) {
            if((existing_lock->lock_mode == EXCLUSIVE || lock_instance->lock_mode == EXCLUSIVE)) {
                if(!consecutive_slocks) {
                    if(existing_lock->lock_mode == SHARED) {
                        consecutive_slocks = true;
                        trx_wait[trx_id].insert(existing_lock->trx_id);
                    } else {
                        trx_wait[trx_id].insert(existing_lock->trx_id);
                        break;
                    }
                } else {
                    if(existing_lock->lock_mode == SHARED) {
                        trx_wait[trx_id].insert(existing_lock->trx_id);
                    } else {
                        break;
                    }
                }
            }
        }
        existing_lock = existing_lock->prev;
    }

    bool should_wait = trx_wait[trx_id].size() > 0;

    if (lock_helper::find_deadlock(trx_id)) {
        trx_wait.erase(trx_id);
        pthread_mutex_unlock(lock_manager_mutex);
        return nullptr;
    }

    // Check if this lock is already acquired.
    // Also check if this lock is compressible.
    existing_lock = lock_instances[lock_location]->tail;
    while(existing_lock) {
        if (existing_lock->trx_id == trx_id && existing_lock->acquired) {
            if(lock_helper::get_bit(existing_lock, key_idx)) {
                // Already acquired lcok
                if (existing_lock->lock_mode == EXCLUSIVE ||
                        lock_mode == SHARED) {
                    pthread_cond_destroy(lock_instance->cond);
                    delete lock_instance->cond;
                    delete lock_instance;
                    pthread_mutex_unlock(lock_manager_mutex);
                    return existing_lock;
                }
            } else if(!should_wait) {
                // Lock compression
                if(existing_lock->lock_mode == SHARED && lock_mode == SHARED) {
                    lock_helper::set_bit(existing_lock, key_idx);
                    pthread_cond_destroy(lock_instance->cond);
                    delete lock_instance->cond;
                    delete lock_instance;
                    pthread_mutex_unlock(lock_manager_mutex);
                    return existing_lock;
                }
            }
        }
        existing_lock = existing_lock->prev;
    }

    lock_instance->next_trx = nullptr;
    lock_instance->next = nullptr;
    lock_instance->prev = lock_instance->list->tail;
    lock_instance->list->tail->next = lock_instance;
    lock_instance->list->tail = lock_instance;

    if (should_wait) {
        pthread_cond_wait(lock_instance->cond, lock_manager_mutex);
    }
    trx_wait.erase(trx_id);

    lock_instance->acquired = true;
    pthread_mutex_unlock(lock_manager_mutex);
    return lock_instance;
};

int lock_release(Lock* lock_obj) {
    pthread_mutex_lock(lock_manager_mutex);

    trx_wait.erase(lock_obj->trx_id);

    pthread_cond_destroy(lock_obj->cond);
    delete lock_obj->cond;

    if (lock_obj->prev != nullptr) {
        lock_obj->prev->next = lock_obj->next;
    }
    if (lock_obj->next != nullptr) {
        lock_obj->next->prev = lock_obj->prev;
    }

    if (lock_obj->list->head == lock_obj) {
        lock_obj->list->head = lock_obj->next;
    }
    if (lock_obj->list->tail == lock_obj) {
        lock_obj->list->tail = lock_obj->prev;
    }

    if (lock_obj->list->head == nullptr) {
        lock_instances.erase(lock_obj->list->lock_location);

        delete lock_obj->list;
        delete lock_obj;

        pthread_mutex_unlock(lock_manager_mutex);
        return 0;
    }

    for(int key_idx = 0; key_idx < 64; key_idx++) {
        if(lock_helper::get_bit(lock_obj, key_idx)) {
            lock_t* next_lock = lock_obj->list->head;
            while(next_lock != nullptr) {
                if(lock_helper::get_bit(next_lock, key_idx) && !next_lock->acquired) {
                    trx_wait[next_lock->trx_id].erase(lock_obj->trx_id);
                    if(trx_wait[next_lock->trx_id].size() == 0) {
                        pthread_cond_signal(next_lock->cond);
                    }
                }
                next_lock = next_lock->next;
            }

            // Multiple lock mask is only for shared locks.
            if(lock_obj->lock_mode == EXCLUSIVE) {
                break;
            }
        }
    }
    
    delete lock_obj;
    pthread_mutex_unlock(lock_manager_mutex);
    return 0;
}
/** @}*/

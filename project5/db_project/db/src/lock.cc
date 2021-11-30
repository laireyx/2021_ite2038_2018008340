/**
 * @addtogroup LockManager
 * @{
 */
#include <lock.h>
#include <transaction.h>

#include <iostream>
#include <new>
#include <unordered_map>
#include <unordered_set>

/// @brief Lock manager mutex.
pthread_mutex_t* lock_manager_mutex;

/** @todo change it to MAX_TABLE_INSTANCE */
std::unordered_map<PageLocation, LockList*> lock_instances;
/// @brief Transaction waiting list.
std::unordered_map<trxid_t, std::unordered_set<trxid_t>> trx_wait;

namespace lock_helper {

tableid_t get_table_id_from_lock(const Lock* lock) {
    return lock->list->lock_location.first;
}
pagenum_t get_page_num_from_lock(const Lock* lock) {
    return lock->list->lock_location.second;
}
bool _find_deadlock(trxid_t current, trxid_t root) {
    if (current == root) {
        return true;
    }
    for (auto& occupant : trx_wait[root]) {
        if (_find_deadlock(occupant, root)) {
            return true;
        }
    }
    return false;
}
bool find_deadlock(trxid_t root) {
    for (auto& occupant : trx_wait[root]) {
        if (_find_deadlock(occupant, root)) {
            return true;
        }
    }
    return false;
}

}  // namespace lock_helper

int init_lock_table() {
    try {
        lock_manager_mutex = new pthread_mutex_t;
        return pthread_mutex_init(lock_manager_mutex, nullptr);
    } catch (std::bad_alloc exception) {
        return -1;
    }
}

Lock* lock_acquire(int table_id, pagenum_t page_id, recordkey_t key,
                   trxid_t trx_id, int lock_mode) {
    pthread_mutex_lock(lock_manager_mutex);
    Lock* lock_instance = new Lock;
    auto lock_location = std::make_pair(table_id, page_id);

    lock_instance->lock_mode = static_cast<LockMode>(lock_mode);
    lock_instance->acquired = false;

    lock_instance->cond = new pthread_cond_t;
    pthread_cond_init(lock_instance->cond, nullptr);

    lock_instance->trx_id = trx_id;
    lock_instance->key = key;

    if (lock_instances.find(lock_location) == lock_instances.end()) {
        LockList* new_lock_list = new LockList;
        new_lock_list->lock_location = lock_location;
        new_lock_list->head = new_lock_list->tail = lock_instance;

        lock_instance->acquired = true;
        lock_instance->list = new_lock_list;
        lock_instance->prev = nullptr;
        lock_instance->next = nullptr;

        lock_instances[lock_location] = new_lock_list;
        pthread_mutex_unlock(lock_manager_mutex);
        return lock_instance;
    }

    auto lock_list = lock_instances[lock_location];

    bool should_wait = false;
    Lock* existing_lock = lock_list->head;

    if (trx_wait.find(trx_id) == trx_wait.end()) {
        trx_wait[trx_id].clear();
    }
    while(existing_lock) {
        if(existing_lock->key == key) {
            if(existing_lock->trx_id != trx_id) {
                if(existing_lock->lock_mode == EXCLUSIVE || lock_mode == EXCLUSIVE) {
                    should_wait = true;
                    if (trx_helper::get_trx_instance(existing_lock->trx_id)
                            .state == WAITING) {
                        trx_wait[trx_id].insert(existing_lock->trx_id);
                    }
                }
            } else {
                if (existing_lock->acquired &&
                    (existing_lock->lock_mode == EXCLUSIVE ||
                     lock_mode == SHARED)) {
                    trx_wait.erase(trx_id);
                    pthread_mutex_unlock(lock_manager_mutex);
                    return existing_lock;
                }
            }
        }
        existing_lock = existing_lock->next;
    }

    if (lock_helper::find_deadlock(trx_id)) {
        pthread_mutex_unlock(lock_manager_mutex);
        return nullptr;
    }

    lock_instance->prev = lock_list->tail;
    lock_instance->next = nullptr;
    lock_instance->next_trx = nullptr;
    lock_list->tail->next = lock_instance;
    lock_list->tail = lock_instance;

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

    pthread_cond_destroy(lock_obj->cond);
    delete lock_obj->cond;

    if (lock_obj->prev != nullptr) {
        lock_obj->prev->next = lock_obj->next;
    } else {
        lock_obj->list->head = lock_obj->next;
    }

    if (lock_obj->next == nullptr) {
        lock_instances.erase(lock_obj->list->lock_location);

        delete lock_obj->list;
        delete lock_obj;

        pthread_mutex_unlock(lock_manager_mutex);
        return 0;
    }

    pthread_cond_signal(lock_obj->next->cond);
    delete lock_obj;
    pthread_mutex_unlock(lock_manager_mutex);
    return 0;
}
/** @}*/
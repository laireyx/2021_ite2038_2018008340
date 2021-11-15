/**
 * @addtogroup LockManager
 * @{
 */
#include <lock.h>

#include <map>
#include <new>

pthread_mutex_t* lock_manager_mutex;

/** @todo change it to MAX_TABLE_INSTANCE */
std::map<LockLocation, lock_t*> lock_instances;

int init_lock_table() {
    try {
        lock_manager_mutex = new pthread_mutex_t;
        return pthread_mutex_init(lock_manager_mutex, nullptr);
    } catch (std::bad_alloc exception) {
        return -1;
    }
}

lock_t* lock_acquire(int table_id, recordkey_t key) {
    pthread_mutex_lock(lock_manager_mutex);
    lock_t* lock_instance = new lock_t;
    auto lock_location = std::make_pair(table_id, key);

    lock_instance->lock_location = lock_location;

    lock_instance->cond = nullptr;

    lock_instance->prev = nullptr;
    lock_instance->next = nullptr;

    if (lock_instances.find(lock_location) == lock_instances.end()) {
        lock_instance->cond = new pthread_cond_t;
        pthread_cond_init(lock_instance->cond, nullptr);

        lock_instances[lock_location] = lock_instance;
        pthread_mutex_unlock(lock_manager_mutex);
        return lock_instance;
    }

    lock_t* lock_tail = lock_instances[lock_location];
    while (lock_tail->next != nullptr) {
        lock_tail = lock_tail->next;
    }

    lock_instance->cond = lock_tail->cond;

    lock_instance->prev = lock_tail;
    lock_tail->next = lock_instance;

    while (true) {
        pthread_cond_wait(lock_instance->cond, lock_manager_mutex);

        if (lock_instance->prev == nullptr) {
            break;
        }
    }

    pthread_mutex_unlock(lock_manager_mutex);
    return lock_instance;
};

int lock_release(lock_t* lock_obj) {
    pthread_mutex_lock(lock_manager_mutex);

    if (lock_obj->next == nullptr) {
        lock_instances.erase(lock_obj->lock_location);

        pthread_cond_destroy(lock_obj->cond);
        delete lock_obj->cond;
        delete lock_obj;

        pthread_mutex_unlock(lock_manager_mutex);
        return 0;
    }

    lock_t* next_lock = lock_obj->next;
    next_lock->prev = nullptr;

    lock_instances[lock_obj->lock_location] = next_lock;
    delete lock_obj;

    pthread_cond_broadcast(next_lock->cond);
    pthread_mutex_unlock(lock_manager_mutex);
    return 0;
}
/** @}*/
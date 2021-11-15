/**
 * @addtogroup LockManager
 * @{
 */
#pragma once

#include <pthread.h>
#include <types.h>

#include <cstdint>
#include <functional>

/**
 * @class lock_t
 * @brief Lock instance.
 */
struct Lock {
    /// @brief Lock table and key information
    LockLocation lock_location;

    /// @brief Conditional variable for this lock.
    pthread_cond_t* cond;

    /// @brief Previous waiting lock.
    Lock* prev;
    /// @brief Next waiting lock.
    Lock* next;
};

namespace std {
template <>
struct hash<LockLocation> {
    size_t operator()(const LockLocation& location) const {
        size_t hash_value = 17;
        hash_value = hash_value * 31 + std::hash<int>()(location.first);
        hash_value = hash_value * 31 + std::hash<int64_t>()(location.second);
        return hash_value;
    }
};
}  // namespace std

typedef struct Lock lock_t;

/* APIs for lock table */

/**
 * @brief
 * @details
 *
 * @return 0 if success, nonzero otherwise.
 */
int init_lock_table();

/**
 * @brief Acquire a lock corresponding to given table id and key.
 * @details If there are no existing lock, it instantly returns a new lock
 * instance. Otherwise, blocks until all previous lock is released and returns.
 *
 * @param table_id  table id.
 * @param key       row key.
 * @return 0 if success, nonzero otherwise.
 */
lock_t* lock_acquire(int table_id, recordkey_t key);

/**
 * @brief Release a lock.
 * @details Releases given lock and wakes up next blocked
 * <code>lock_acquire()</code>.
 *
 * @param lock_obj  lock object acquired by <code>lock_acquire()</code>.
 * @return 0 if success, nonzero otherwise.
 */
int lock_release(lock_t* lock_obj);
/** @}*/
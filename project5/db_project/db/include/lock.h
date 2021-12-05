/**
 * @addtogroup LockManager
 * @{
 */
#pragma once

#include <pthread.h>
#include <types.h>

#include <cstdint>
#include <functional>

enum LockMode { SHARED = 0, EXCLUSIVE = 1 };
struct Lock;
/**
 * @class LockList
 * @brief Lock list.
 */
struct LockList {
    /// @brief Lock table and key information
    LockLocation lock_location;

    /// @brief Head of lock list.
    Lock* head;
    /// @brief Tail of lock list.
    Lock* tail;
};

/**
 * @class lock_t
 * @brief Lock instance.
 */
struct Lock {
    /// @brief Lock mode.
    LockMode lock_mode;
    /// @brief <code>true</code> if acquired, <code>false</code> if sleeping.
    bool acquired;

    /// @brief Conditional variable for this lock.
    pthread_cond_t* cond;

    /// @brief Lock list.
    LockList* list;

    /// @brief Transaction id.
    trxid_t trx_id;
    /// @brief Lock mask for shared lock compression.
    lockmask_t mask;

    /// @brief Previous waiting lock.
    Lock* prev;
    /// @brief Next waiting lock.
    Lock* next;
    /// @brief Next transaction lock.
    Lock* next_trx;
};

namespace lock_helper {

/**
 * @brief Get the pos-th bit of the lock key mask.
 *
 * @param lock  lock object.
 * @param pos   position.
 * @return 0 or 1.
 */
constexpr int get_bit(Lock* lock, int pos) {
    return (lock->mask & (uint64_t(1) << pos)) >> pos;
}
/**
 * @brief Set the pos-th bit of the lock key mask.
 *
 * @param lock  lock object.
 * @param pos   position.
 * @return 0 or 1.
 */
constexpr int set_bit(Lock* lock, int pos) {
    return lock->mask |= (uint64_t(1) << pos);
}
/**
 * @brief Clear the pos-th bit of the mask.
 *
 * @param mask  bit mask.
 * @param pos   position.
 * @return 0 or 1.
 */
constexpr int clear_bit(uint64_t& mask, int pos) {
    return mask |= ~(uint64_t(1) << pos);
}

}  // namespace lock_helper

/* APIs for lock table */

/**
 * @brief Initialize lock table.
 *
 * @return <code>0</code> if success, negative value otherwise.
 */
int init_lock_table();

/**
 * @brief Cleanup lock table.
 *
 * @return <code>0</code> if success, negative value otherwise.
 */
int cleanup_lock_table();

/**
 * @brief Create an acquired explicit lock.
 * @details Implicit locking is only for X-lock, so we can safely assume that created lock is
 * always has <code>EXCLUSIVE</code> lock mode.
 * 
 * @param table_id  table id.
 * @param page_id   page id.
 * @param key_idx   record key index.
 * @param trx_id    transaction id.
 * @return created lock instance.
 */
Lock* explicit_lock(tableid_t table_id, pagenum_t page_id, int key_idx, trxid_t trx_id);

/**
 * @brief Acquire a lock corresponding to given table id and key.
 * @details If there are no existing lock, it instantly returns a new lock
 * instance. Otherwise, blocks until all previous lock is released and returns.
 *
 * @param table_id  table id.
 * @param page_id   page id.
 * @param key_idx   record key index.
 * @param trx_id    transaction id.
 * @param lock_mode lock mode.
 * @return non-null lock instance if success, <code>nullptr</code> otherwise.
 */
Lock* lock_acquire(tableid_t table_id, pagenum_t page_id, int key_idx,
                   trxid_t trx_id, int lock_mode);

/**
 * @brief Release a lock.
 * @details Releases given lock and wakes up next blocked
 * <code>lock_acquire()</code>.
 *
 * @param lock_obj  lock object acquired by <code>lock_acquire()</code>.
 * @return 0 if success, nonzero otherwise.
 */
int lock_release(Lock* lock_obj);

bool empty_trx(trxid_t trx_id);

typedef struct Lock lock_t;
/** @}*/
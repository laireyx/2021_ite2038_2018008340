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
struct LockList {
    /// @brief Lock table and key information
    PageLocation lock_location;

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

    /// @brief Conditional variable for this lock.
    pthread_cond_t* cond;

    /// @brief Lock list.
    LockList* list;

    /// @brief Transaction id.
    trxid_t trx_id;
    /// @brief Lock mask.
    lockmask_t mask;

    /// @brief Previous waiting lock.
    Lock* prev;
    /// @brief Next waiting lock.
    Lock* next;
    /// @brief Previous transaction lock.
    Lock* prev_trx;
    /// @brief Next transaction lock.
    Lock* next_trx;
};

namespace lock_helper {

/**
 * @brief Get the pos-th bit of the mask.
 *
 * @param mask  bit mask.
 * @param pos   position.
 * @return 0 or 1.
 */
constexpr int get_bit(uint64_t mask, int pos) {
    return (mask & (uint64_t(1) << pos)) >> pos;
}
/**
 * @brief Set the pos-th bit of the mask.
 *
 * @param mask  bit mask.
 * @param pos   position.
 * @return 0 or 1.
 */
constexpr int set_bit(uint64_t& mask, int pos) {
    return mask |= (uint64_t(1) << pos);
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
/**
 * @brief Get table id of the lock.
 *
 * @param lock  lock instance.
 * @return      table id.
 */
tableid_t get_table_id_from_lock(const Lock* lock);
/**
 * @brief Get page number of the lock.
 *
 * @param lock  lock instance.
 * @return      page number.
 */
pagenum_t get_page_num_from_lock(const Lock* lock);

}  // namespace lock_helper

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
 * @param page_id   page id.
 * @param key       row key.
 * @param trx_id    transaction id.
 * @param lock_mode lock mode.
 * @return non-null lock instance if success, <code>nullptr</code> otherwise.
 */
Lock* lock_acquire(int table_id, pagenum_t page_id, recordkey_t key,
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

typedef struct Lock lock_t;
/** @}*/
/**
 * @addtogroup LockManager
 * @{
 */
#pragma once

#include <pthread.h>
#include <types.h>

#include <cstdint>
#include <functional>
#include <unordered_set>

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
    /// @details There can be multiple locks with same location, except for eXclusive lock. It means, if there were any X lock, then any other lock can be acquired at the same location and it is why X lock is named "exclusive".
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
 * @brief Recursively traverse transaction waiting graph.
 * @details Calls itself for every child nodes of <code>current</code> node of transaction waiting graph.
 * If any child that represents <code>root</code>, then it means there is a cycle.
 * Recursion ends at <code>0</code>, which means there are no children.
 * 
 * @param current   current transactio node
 * @param root      root transaction node
 * @param visit     visit check(for recursive traversal)
 * @return <code>true</code> if found, <code>false</code> otherwise.
 */
bool _find_deadlock(trxid_t current, trxid_t root, std::unordered_set<trxid_t> visit);
/**
 * @brief Determine if there is any deadlock includes this transaction.
 * @details Every <code>lock_acquire()</code> request updates a deadlock waiting graph(transaction waiting list),
 * and this function traverses the graph to find the cycle includes target transaction.
 * 
 * @param root  root transaction.
 * @return <code>true</code> if found, <code>false</code> otherwise.
 */
bool find_deadlock(trxid_t root);

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
 * @brief Acquire a lock corresponding to given table id and key.
 * @details If there are no existing lock, it instantly returns a new lock
 * instance. Otherwise, blocks until all previous lock is released and returns.
 * If the transaction needs blocking, then it will create transaction waiting graph edges,
 * which points the transactions which are owning conflicting leading locks.
 * However, if this lock request induces a new deadlock, then it instantly returns
 * nullptr, to abort this transaction and other workers keep going on.
 *
 * @param table_id  table id.
 * @param page_id   page id.
 * @param key       record key index.
 * @param trx_id    transaction id.
 * @param lock_mode lock mode.
 * @return non-null lock instance if success, <code>nullptr</code> otherwise.
 */
Lock* lock_acquire(int table_id, pagenum_t page_id, int key_idx,
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
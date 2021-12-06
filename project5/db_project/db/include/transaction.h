/**
 * @addtogroup TransactionManager
 * @{
 */
#pragma once

#include <lock.h>
#include <types.h>

#include <vector>

/**
 * @brief Transaction log type.
 */
enum LogType { UPDATE = 0 };

/**
 * @brief Transaction log.
 *
 */
struct TransactionLog {
    /// @brief Log type.
    LogType type;
    /// @brief Updated table id.
    tableid_t table_id;
    /// @brief Updated record key.
    recordkey_t key;
    /// @brief Old record value.
    char* old_value;
    /// @brief Old record value size.
    valsize_t old_val_size;
    /// @brief Previous transaction log index.
    trxlogid_t prev_trx_log;
};

/**
 * @brief Transaction running state.
 */
enum TransactionState { RUNNING = 0, WAITING = 1, COMMITTING = 2, COMMITTED = 3, ABORTING = 4, ABORTED = 5 };

/**
 * @brief Transaction instance.
 *
 */
struct TransactionInstance {
    /// @brief Transaction running state.
    TransactionState state;
    /// @brief The last log of this transaction.
    trxlogid_t log_tail;
    /// @brief Transaction lock head.
    Lock* lock_head;
    /// @brief Transaction lock tail.
    Lock* lock_tail;
};

typedef struct TransactionLog trxlog_t;

namespace trx_helper {

/**
 * @brief Get the trx instance object
 * @details Quickfix. Expects dragon ahead.
 *
 * @param trx_id    transaction id.
 * @return          transaction instance.
 */
TransactionInstance& get_trx_instance(trxid_t trx_id);

/**
 * @brief Wrapper for <code>lock_acquire()</code>
 * 
 * @param table_id  table id.
 * @param page_idx  page index.
 * @param key       record key index.
 * @param trx_id    transaction id.
 * @param lock_mode lock mode.
 * @return          acquired lock.
 */
lock_t* lock_acquire(int table_id, pagenum_t page_idx, int key_idx,
                   trxid_t trx_id, int lock_mode);
/**
 * @brief Verify if transaction is on good state.
 * @details check if the transaction is already finished(by commit or abort).
 *
 * @param instance  transaction instance.
 * @return <code>true</code> if instance is good to go. <code>false</code>
 * otherwise.
 */
bool verify_trx(const TransactionInstance& instance);
/**
 * @brief Instantiate a transaction and return its id.
 *
 * @return transaction id.
 */
trxid_t new_trx_instance();
/**
 * @brief Release all the locks in the instance.
 *
 * @param instance transaction instance.
 */
void release_trx_locks(TransactionInstance& instance);
/**
 * @brief Rollback an unfinished transaction and finish it.
 * @details Roll back all the updates provoked by this transaction.
 *
 * @param trx_id  transaction id.
 */
void trx_rollback(trxid_t trx_id);
/**
 * @brief Flush all the transaction log(for recovery.)
 * @details Not used for this time(maybe for project6).
 */
void flush_trx_log();
/**
 * @brief Immediately abort a transaction and release all of its locks.
 * @details Rolls back all the updates, flushes transaction log, and finally
 * releases its own lock.
 *
 * @param trx_id transaciton id obtained with <code>trx_begin()</code>.
 */
void trx_abort(trxid_t trx_id);
/**
 * @brief Log an update query into transaction log.
 *
 * @param table_id      table id.
 * @param key           record key.
 * @param old_value     old record value.
 * @param old_val_size  old value size.
 * @param trx_id        transaction id.
 * @return              created log id.
 */
trxlogid_t log_update(tableid_t table_id, recordkey_t key,
                      const char* old_value, valsize_t old_val_size,
                      trxid_t trx_id);
};  // namespace trx_helper

/**
 * @brief Initialize a transaction manager.
 * 
 * @return <code>0</code> if success, negative value otherwise.
 */
int init_trx();

/**
 * @brief Cleanup a transaction manager.
 *
 * @return <code>0</code> if success, negative value otherwise.
 */
int cleanup_trx();

/**
 * @brief Begin a transaction.
 *
 * @return positive transaction id if success. <code>0</code> or negative
 * otherwise.
 */
trxid_t trx_begin();
/**
 * @brief Commit a transaction.
 * @details Flushes a log and releases its own locks.
 *
 * @param trx_id transaction id obtained with <code>trx_begin()</code>.
 * @return <code>trx_id</code>(committed transaction id) if success.
 * <code>0</code> otherwise.
 */
trxid_t trx_commit(trxid_t trx_id);

/** @}*/
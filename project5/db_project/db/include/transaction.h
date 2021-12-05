/**
 * @addtogroup DiskSpaceManager
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
enum TransactionState { RUNNING = 0, COMMITTING = 1, ABORTING = 2 };

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
 * @brief Connect a lock to the tail of next-transaction-lock list.
 * 
 * @param trx_id    transaction id.
 * @param lock      lock object.
 * @return <code>true</code> if success, <code>false</code> if otherwise.
 */
bool connect_lock_tail(trxid_t trx_id, lock_t* lock);
/**
 * @brief Check if target transaction is running.
 * @details RUNNING, COMMITTING, ABORTING transaction is running transaction.
 * 
 * @param trx_id    transaction id.
 * @return <code>true</code> if transaction is running.
 */
bool is_trx_running(trxid_t trx_id);
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
 * @param trx_id transaction id.
 */
void release_trx_locks(trxid_t trx_id);
/**
 * @brief Rollback an unfinished transaction and finish it.
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
 *
 * @param trx_id transaction id obtained with <code>trx_begin()</code>.
 * @return <code>trx_id</code>(committed transaction id) if success.
 * <code>0</code> otherwise.
 */
trxid_t trx_commit(trxid_t trx_id);

/** @}*/
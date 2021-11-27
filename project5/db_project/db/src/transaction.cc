/**
 * @addtogroup DiskSpaceManager
 * @{
 */
#include <transaction.h>
#include <tree.h>

#include <cstring>
#include <map>

/// @brief Accumulated trx id.
trxid_t accumulated_trx_id = 0;
/// @brief Accumulated trx log id.
trxid_t accumulated_trxlog_id = 0;
/// @brief Transaction instances.
std::unordered_map<trxid_t, TransactionInstance> transaction_instances;
/// @brief Transaction log.
std::unordered_map<trxlogid_t, TransactionLog> trx_log;

namespace trx_helper {

bool verify_trx(const TransactionInstance& instance) {
    return !instance.is_finished;
}

trxid_t new_trx_instance() {
    TransactionInstance instance;
    instance.is_finished = false;
    instance.lock_head = instance.lock_tail = nullptr;
    transaction_instances[++accumulated_trx_id] = instance;

    return accumulated_trx_id;
}

void release_trx_locks(TransactionInstance& instance) {
    lock_t* lock_ptr = instance.lock_head;
    while (lock_ptr != nullptr) {
        lock_t* next_lock = lock_ptr->next;
        lock_release(lock_ptr);
        lock_ptr = next_lock;
    }
}

void trx_rollback(trxid_t trx_id) {
    TransactionInstance& instance = transaction_instances[trx_id];
    if (!verify_trx(instance)) return;

    trxid_t current_log_id = instance.log_tail;
    while (current_log_id != 0) {
        TransactionLog& log = trx_log[current_log_id];
        update_node(log.table_id, log.key, log.old_value, log.old_val_size,
                    nullptr, trx_id);
    }
}

void flush_trx_log() {
    /// @todo todo on recovery.
}

void trx_abort(trxid_t trx_id) {
    TransactionInstance& instance = transaction_instances[trx_id];

    trx_rollback(trx_id);
    flush_trx_log();
    release_trx_locks(instance);

    transaction_instances.erase(trx_id);
}

trxlogid_t log_update(tableid_t table_id, recordkey_t key,
                      const char* old_value, valsize_t old_val_size,
                      trxid_t trx_id) {
    TransactionLog update_log;
    TransactionInstance& instance = transaction_instances[trx_id];

    update_log.type = UPDATE;
    update_log.table_id = table_id;
    update_log.key = key;
    memcpy(update_log.old_value, old_value, old_val_size);
    update_log.old_val_size = old_val_size;

    trx_log[++accumulated_trxlog_id] = update_log;

    // connect a trx log list.
    trx_log[instance.log_tail].prev_trx_log = instance.log_tail;
    if (instance.log_tail == 0) {
        instance.log_tail = accumulated_trxlog_id;
    }

    return accumulated_trxlog_id;
}

}  // namespace trx_helper

trxid_t trx_begin() { return trx_helper::new_trx_instance(); }

trxid_t trx_commit(trxid_t trx_id) {
    TransactionInstance& instance = transaction_instances[trx_id];

    if (!trx_helper::verify_trx(instance)) return 0;

    trx_helper::release_trx_locks(instance);
    trx_helper::flush_trx_log();

    return trx_id;
}

/** @}*/
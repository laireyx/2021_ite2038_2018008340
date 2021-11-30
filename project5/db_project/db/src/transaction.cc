/**
 * @addtogroup DiskSpaceManager
 * @{
 */
#include <pthread.h>
#include <transaction.h>
#include <tree.h>

#include <cstring>
#include <map>
#include <new>

/// @brief Transaction manager mutex.
pthread_mutex_t* trx_manager_mutex = nullptr;

/// @brief Accumulated trx id.
trxid_t accumulated_trx_id = 0;
/// @brief Accumulated trx log id.
trxid_t accumulated_trxlog_id = 0;
/// @brief Transaction instances.
std::unordered_map<trxid_t, TransactionInstance> transaction_instances;
/// @brief Transaction log.
std::unordered_map<trxlogid_t, TransactionLog> trx_log;

namespace trx_helper {

TransactionInstance& get_trx_instance(trxid_t trx_id) {
    return transaction_instances[trx_id];
}
lock_t* lock_acquire(int table_id, pagenum_t page_id, recordkey_t key,
                   trxid_t trx_id, int lock_mode) {
    pthread_mutex_lock(trx_manager_mutex);
    TransactionInstance& instance = transaction_instances[trx_id];

    instance.state = WAITING;
    pthread_mutex_unlock(trx_manager_mutex);
    
    lock_t* lock;
    if(!(lock = ::lock_acquire(table_id, page_id, key, trx_id, lock_mode))) {
        // Lock failed. abort this transaction.
        trx_abort(trx_id);
        return nullptr;
    }

    pthread_mutex_lock(trx_manager_mutex);
    if (instance.lock_head == nullptr) {
        instance.lock_head = lock;
        instance.lock_tail = lock;
    } else {
        instance.lock_tail->next_trx = lock;
        instance.lock_tail = lock;
    }

    instance.state = RUNNING;
    pthread_mutex_unlock(trx_manager_mutex);
    return lock;
}

bool verify_trx(const TransactionInstance& instance) {
    return instance.state == RUNNING;
}

trxid_t new_trx_instance() {
    TransactionInstance instance;
    instance.state = RUNNING;
    instance.lock_head = instance.lock_tail = nullptr;
    transaction_instances[++accumulated_trx_id] = instance;

    return accumulated_trx_id;
}

void release_trx_locks(TransactionInstance& instance) {
    lock_t* lock_ptr = instance.lock_head;
    while (lock_ptr != nullptr) {
        lock_t* next_lock = lock_ptr->next_trx;
        lock_release(lock_ptr);
        lock_ptr = next_lock;
    }
}

void trx_rollback(trxid_t trx_id) {
    TransactionInstance& instance = transaction_instances[trx_id];
    if (!verify_trx(instance)) {
        return;
    }

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

    instance.state = ABORTING;
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

int init_trx() {
    try {
        if (trx_manager_mutex != nullptr) {
            return -1;
        }
        trx_manager_mutex = new pthread_mutex_t;
        if (pthread_mutex_init(trx_manager_mutex, nullptr)) {
            return -1;
        }
        transaction_instances.clear();
    } catch (std::bad_alloc exception) {
        return -1;
    }
    return 0;
}

int cleanup_trx() {
    if (trx_manager_mutex) {
        pthread_mutex_destroy(trx_manager_mutex);
        delete trx_manager_mutex;
        trx_manager_mutex = nullptr;
    }
    return 0;
}

trxid_t trx_begin() {
    pthread_mutex_lock(trx_manager_mutex);
    trxid_t created_trx = trx_helper::new_trx_instance();
    pthread_mutex_unlock(trx_manager_mutex);
    return created_trx;
}

trxid_t trx_commit(trxid_t trx_id) {
    pthread_mutex_lock(trx_manager_mutex);
    TransactionInstance& instance = transaction_instances[trx_id];

    if (!trx_helper::verify_trx(instance)) {
        pthread_mutex_unlock(trx_manager_mutex);
        return 0;
    }

    instance.state = COMMITTING;
    trx_helper::release_trx_locks(instance);
    trx_helper::flush_trx_log();
    instance.state = COMMITTED;

    transaction_instances.erase(trx_id);
    pthread_mutex_unlock(trx_manager_mutex);
    return trx_id;
}

/** @}*/

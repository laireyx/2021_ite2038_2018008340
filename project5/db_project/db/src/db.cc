/**
 * @addtogroup DatabaseAPI
 * @{
 */
#include <buffer.h>
#include <db.h>
#include <tree.h>
#include <lock.h>

#include <cstring>

int init_db(int num_buf) {
    if(init_lock_table() != 0) return -1;
    if(init_buffer(num_buf) != 0) return -1;
    return 0;
}

tableid_t open_table(char* pathname) {
    return buffered_open_table_file(pathname);
}

int db_insert(tableid_t table_id, recordkey_t key, char* value,
              valsize_t value_size) {
    return insert_node(table_id, key, value, value_size) != 0 ? 0 : -1;
}

int db_find(tableid_t table_id, recordkey_t key, char* ret_val,
            valsize_t* value_size) {
    if (!find_by_key(table_id, key, ret_val, value_size, 0)) {
        return -1;
    }
    return 0;
}

int db_find(tableid_t table_id, recordkey_t key, char* ret_val,
            valsize_t* value_size, trxid_t trx_id) {
    if (!find_by_key(table_id, key, ret_val, value_size, trx_id)) {
        return -1;
    }
    return 0;
}

int db_update(tableid_t table_id, recordkey_t key, char* value,
              valsize_t new_val_size, valsize_t* old_val_size, trxid_t trx_id) {
    return update_node(table_id, key, value, new_val_size, old_val_size,
                       trx_id);
}

int db_delete(tableid_t table_id, recordkey_t key) {
    if (!delete_node(table_id, key)) {
        return -1;
    }
    return 0;
}

int shutdown_db() {
    shutdown_buffer();
    return 0;
}
/** @}*/
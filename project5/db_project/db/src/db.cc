/**
 * @addtogroup DatabaseAPI
 * @{
 */
#include <buffer.h>
#include <db.h>
#include <tree.h>

#include <cstring>

int init_db(int num_buf) { return init_buffer(num_buf); }

tableid_t open_table(char* pathname) {
    return buffered_open_table_file(pathname);
}

int db_insert(tableid_t table_id, recordkey_t key, char* value,
              uint16_t value_size) {
    return insert_node(table_id, key, value, value_size) != 0 ? 0 : -1;
}

int db_find(tableid_t table_id, recordkey_t key, char* ret_val,
            uint16_t* value_size) {
    if (!find_by_key(table_id, key, ret_val, value_size)) {
        return -1;
    }
    return 0;
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
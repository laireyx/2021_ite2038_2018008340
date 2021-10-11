#include "table.h"

#include "file.h"
#include "tree.h"

#include <cstring>

tableid_t open_table(const char* pathname) {
    return file_open_table_file(pathname);
}

int db_insert(tableid_t table_id, int64_t key, const char* value,
              uint16_t value_size) {
    return insert_node(table_id, key, value, value_size) != 0 ? 0 : -1;
}

int db_find(tableid_t table_id, int64_t key, char* ret_val,
            uint16_t* value_size) {
    char* find_result = find_by_key(table_id, key, value_size);
    if(find_result == nullptr) {
        return -1;
    }

    memcpy(ret_val, find_result, *value_size);
    delete[] find_result;

    return 0;
}

int db_delete(tableid_t table_id, int64_t key) { return 0; }
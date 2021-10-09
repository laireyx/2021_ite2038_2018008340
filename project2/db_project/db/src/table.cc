#include "table.h"

#include "file.h"

namespace table_helper {
bool switch_to_id(int64_t table_id) { return true; }
}  // namespace table_helper

tableid_t open_table(const char* pathname) {
    return file_open_table_file(pathname);
}

int db_insert(tableid_t table_id, int64_t key, const char* value,
              uint16_t value_size) {
    return 0;
}

int db_find(tableid_t table_id, int64_t key, char* ret_val,
            uint16_t* value_size) {
    return 0;
}

int db_delete(tableid_t table_id, int64_t key) { return 0; }
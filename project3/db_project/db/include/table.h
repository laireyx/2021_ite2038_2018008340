/**
 * @addtogroup TableManager
 * @{
 */
#pragma once

#include "types.h"

namespace table_helper {
/**
 * @brief   Switch current table into given table id.
 * @details If current table_fd is already means given table id, then do
 * nothing. If not, change table_fd to given fd and re-read header_page from it.
 *
 * @param table_id  table id obtained with
 *                  <code>file_open_table_file()</code>.
 */
bool switch_to_id(int64_t table_id);
}  // namespace table_helper

/**
 * @brief   Initialize database management system.
 *
 * @returns If success, return 0. Otherwise return non-zero value.
 */
int init_db();

/**
 * @brief   Open existing data file using ‘pathname’ or create one if not
 * existed.
 *
 * @param pathname Table file path.
 * @returns unique table id which represents the own table in this database.
 *          return negative value otherwise.
 */
tableid_t open_table(char* pathname);

/**
 * @brief   Insert input (key, value) record with its size to data file at the
 * right place.
 *
 * @param table_id      Table id obtained with <code>open_table()</code>.
 * @param key           Record key.
 * @param value         Record value.
 * @param value_size    Record value size.
 * @returns 0 if success.
 *          negative value otherwise.
 */
int db_insert(tableid_t table_id, int64_t key, char* value,
              uint16_t value_size);

/**
 * @brief   Find the record corresponding the input key.
 * @details If found, ret_val and value_size will be set to matching value and
 * its size.
 *
 * @param       table_id      Table id obtained with <code>open_table()</code>.
 * @param       key           Record key.
 * @param[out]  value         Record value.
 * @param[out]  value_size    Record value size.
 * @returns 0 if success.
 *          negative value otherwise.
 */
int db_find(tableid_t table_id, int64_t key, char* ret_val,
            uint16_t* value_size);

/**
 * @brief   Find the matching record and delete it if found.
 *
 * @param table_id      Table id obtained with <code>open_table()</code>.
 * @param key           Record key.
 * @returns 0 if success.
 *          negative value otherwise.
 */
int db_delete(tableid_t table_id, int64_t key);

/**
 * @brief   Shutdown database management system.
 *
 * @returns If success, return 0. Otherwise return non-zero value.
 */
int shutdown_db();
/** @}*/
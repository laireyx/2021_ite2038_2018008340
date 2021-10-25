/**
 * @addtogroup DatabaseAPI
 * @{
 */
#pragma once

#include "types.h"

/**
 * @brief   Initialize database management system.
 *
 * @returns If success, return 0. Otherwise return non-zero value.
 */
int init_db(int num_buf = 1024);

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
 * @param table_id      table id obtained with <code>open_table()</code>.
 * @param key           record key.
 * @param value         record value.
 * @param value_size    record value size.
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
 * @param       table_id        table id obtained with
 * <code>open_table()</code>.
 * @param       key             record key.
 * @param[out]  value           record value.
 * @param[out]  value_size      record value size.
 * @returns                     0 if success. negative value otherwise.
 */
int db_find(tableid_t table_id, int64_t key, char* ret_val,
            uint16_t* value_size);

/**
 * @brief   Find the matching record and delete it if found.
 *
 * @param table_id      table id obtained with <code>open_table()</code>.
 * @param key           record key.
 * @returns             0 if success. negative value otherwise.
 */
int db_delete(tableid_t table_id, int64_t key);

/**
 * @brief   Shutdown database management system.
 *
 * @returns If success, return 0. Otherwise return non-zero value.
 */
int shutdown_db();
/** @}*/
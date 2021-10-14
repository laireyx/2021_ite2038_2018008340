/**
 * @addtogroup Disk based B+ tree
 * @{
 */
#pragma once

#include "types.h"

constexpr int REDISTRIBUTE_THRESHOLD = 2500;
constexpr int MAX_VALUE_SIZE = 112;

/**
 * @brief Allocate and make a leaf page.
 *
 * @param table_id          Table id.
 * @param parent_page_idx   Parent page index.
 * @returns Created page index.
 */
pagenum_t make_leaf(tableid_t table_id, pagenum_t parent_page_idx = 0);
/**
 * @brief Allocate and make an internal page.
 *
 * @param table_id          Table id.
 * @param parent_page_idx   Parent page index.
 * @returns Created page index.
 */
pagenum_t make_node(tableid_t table_id, pagenum_t parent_page_idx = 0);

/**
 * @brief Create a new tree.
 * @details Create a new leaf page for root page and set initial record.
 *
 * @param table_id      Table id.
 * @param key           Initial record key.
 * @param value         Initial record value.
 * @param value_size    Initial record value size.
 * @returns Root page index.
 */
pagenum_t create_tree(tableid_t table_id, int64_t key, const char* value,
                      uint16_t value_size);

/**
 * @brief Find a leaf node which continas
 *
 * @param table_id      Table id.
 * @param key           Key to query with.
 * @returns Page index if found.
 *          <code>0</code> if the key does not exist.
 */
pagenum_t find_leaf(tableid_t table_id, int64_t key);
/**
 * @brief Find a record with key
 *
 * @param table_id      Table id.
 * @param key           Key to query with.
 * @param value         If the record is found successful, then this value is
 * set to record value. Caller should allocate enough(MAX_VALUE_SIZE) memory for
 * it.
 * @param value_size    If the record is found successful, then this value is
 * set to record size.
 * @return <code>true</code> if found successful. <code>false</code> otherwise.
 */
bool find_by_key(tableid_t table_id, int64_t key, char* value = nullptr,
                 uint16_t* value_size = nullptr);

/**
 * @brief Insert a <code>(key, right_page_idx)</code> tuple in parent page.
 *
 * @param table_id          Table id.
 * @param parent_page_idx   Parent page index.
 * @param left_page_idx     Left page index.
 * @param key               Key which means right page.
 * @param right_page_idx    Right page index.
 * @returns                 Root page number.
 */
pagenum_t insert_into_node(tableid_t table_id, pagenum_t parent_page_idx,
                           pagenum_t left_page_idx, int64_t key,
                           pagenum_t right_page_idx);
/**
 * @brief Insert a <code>(key, right_page_idx)</code> tuple in parent page, and
 * split it into two pages.
 *
 * @param table_id          Table id.
 * @param parent_page_idx   Parent page index.
 * @param key               Key which means right page.
 * @param right_page_idx    Right page index.
 * @returns                 Root page number.
 */
pagenum_t insert_into_node_after_splitting(tableid_t table_id,
                                           pagenum_t parent_page_idx, int64_t key,
                                           pagenum_t right_page_idx);
/**
 * @brief Choose right method between <code>insert_into_node</code> and
 * <code>insert_into_node_after_splitting</code> and call it.
 *
 * @param table_id          Table id.
 * @param left_page_idx     Left page index.
 * @param key               Key which means right page.
 * @param right_page_idx    Right page index.
 * @returns                 Root page number.
 */
pagenum_t insert_into_parent(tableid_t table_id, pagenum_t left_page_idx, int64_t key,
                             pagenum_t right_page_idx);
/**
 * @brief Insert <code>(key, value)</code>into leaf node and split it into two
 * pages.
 *
 * @param table_id          Table id.
 * @param left_page_idx     Left page index.
 * @param key               Record key.
 * @param value             Record value.
 * @param value_size        Record value size.
 * @returns                 Root page number.
 */
pagenum_t insert_into_leaf_after_splitting(tableid_t table_id,
                                           pagenum_t leaf_page_idx, int64_t key,
                                           const char* value,
                                           uint16_t value_size);

/**
 * @brief Choose right method between <code>insert_into_node</code> and
 * <code>insert_into_node_after_splitting</code> and call it.
 *
 * @param table_id          Table id.
 * @param left_page_idx     Left page index.
 * @param key               Key which means right page.
 * @param right_page_idx    Right page index.
 * @returns                 Root page number.
 */
pagenum_t insert_node(tableid_t table_id, int64_t key, const char* value,
                      uint16_t value_size);

pagenum_t adjust_root(tableid_t table_id);
pagenum_t coalesce_internal_nodes(tableid_t table_id, pagenum_t left_page_idx,
                                  int64_t seperate_key,
                                  int seperate_key_idx,
                                  pagenum_t right_page_idx);
pagenum_t coalesce_leaf_nodes(tableid_t table_id, pagenum_t left_page_idx, pagenum_t right_page_idx);
pagenum_t delete_internal_key(tableid_t table_id, pagenum_t internal_page_idx, int64_t key);
pagenum_t delete_leaf_key(tableid_t table_id, pagenum_t leaf_page_idx, int64_t key);
pagenum_t delete_node(tableid_t table_id, int64_t key);
/** @}*/
/**
 * @addtogroup IndexManager
 * @{
 */
#pragma once

#include <types.h>

constexpr int REDISTRIBUTE_THRESHOLD = 2500;
constexpr int MAX_VALUE_SIZE = 112;

/**
 * @brief Allocate and make a leaf page.
 *
 * @param table_id          table id.
 * @param parent_page_idx   parent page index.
 * @returns                 created page index.
 */
pagenum_t make_leaf(tableid_t table_id, pagenum_t parent_page_idx = 0);
/**
 * @brief Allocate and make an internal page.
 *
 * @param table_id          table id.
 * @param parent_page_idx   parent page index.
 * @returns                 created page index.
 */
pagenum_t make_node(tableid_t table_id, pagenum_t parent_page_idx = 0);

/**
 * @brief Create a new tree.
 * @details Create a new leaf page for root page and set initial record.
 *
 * @param table_id      table id.
 * @param key           initial record key.
 * @param value         initial record value.
 * @param value_size    initial record value size.
 * @returns             root page index.
 */
pagenum_t create_tree(tableid_t table_id, recordkey_t key, const char* value,
                      valsize_t value_size);

/**
 * @brief Find a leaf node which contains given key.
 *
 * @param table_id          table id.
 * @param key               key to query with.
 * @returns                 page index if found.
 *                          <code>0</code> if the key does not exist.
 */
pagenum_t find_leaf(tableid_t table_id, recordkey_t key);
/**
 * @brief Find a record with key
 *
 * @param table_id          table id.
 * @param key               key to query with.
 * @param[out] value        If the record is found successful, then this value
 * is set to record value. Caller should allocate enough(MAX_VALUE_SIZE) memory
 * for it.
 * @param[out] value_size   If the record is found successful, then this value
 * is set to record size.
 * @param trx_id            transaction id.
 * @return                  <code>true</code> if found successful.
 * <code>false</code> otherwise.
 */
bool find_by_key(tableid_t table_id, recordkey_t key, char* value = nullptr,
                 valsize_t* value_size = nullptr, trxid_t trx_id = 0);

/**
 * @brief Insert a <code>(key, right_page_idx)</code> tuple in parent page.
 *
 * @param table_id          table id.
 * @param parent_page_idx   parent page index.
 * @param left_page_idx     left page index.
 * @param key               key which means right page.
 * @param right_page_idx    right page index.
 * @returns                 root page number.
 */
pagenum_t insert_into_node(tableid_t table_id, pagenum_t parent_page_idx,
                           pagenum_t left_page_idx, recordkey_t key,
                           pagenum_t right_page_idx);
/**
 * @brief Insert a <code>(key, right_page_idx)</code> tuple in parent page, and
 * split it into two pages.
 *
 * @param table_id          table id.
 * @param parent_page_idx   parent page index.
 * @param key               key which means right page.
 * @param right_page_idx    right page index.
 * @returns                 root page number.
 */
pagenum_t insert_into_node_after_splitting(tableid_t table_id,
                                           pagenum_t parent_page_idx,
                                           recordkey_t key,
                                           pagenum_t right_page_idx);
/**
 * @brief Choose right method between just inserting and
 * <code>insert_into_node_after_splitting</code> and call it.
 *
 * @param table_id          table id.
 * @param left_page_idx     left page index.
 * @param key               key which means right page.
 * @param right_page_idx    right page index.
 * @returns                 root page number.
 */
pagenum_t insert_into_parent(tableid_t table_id, pagenum_t left_page_idx,
                             recordkey_t key, pagenum_t right_page_idx);
/**
 * @brief Insert <code>(key, value)</code>into leaf node and split it into two
 * pages.
 *
 * @param table_id          table id.
 * @param leaf_page_idx     leaf page index.
 * @param key               record key.
 * @param value             record value.
 * @param value_size        record value size.
 * @returns                 root page number.
 */
pagenum_t insert_into_leaf_after_splitting(tableid_t table_id,
                                           pagenum_t leaf_page_idx,
                                           recordkey_t key, const char* value,
                                           valsize_t value_size);

/**
 * @brief Find appropriate leaf page and insert a record into it.
 *
 * @param table_id          table id.
 * @param key               record key.
 * @param value             record value.
 * @param value_size        record value size.
 * @returns                 root page number.
 */
pagenum_t insert_node(tableid_t table_id, recordkey_t key, const char* value,
                      valsize_t value_size);

/**
 * @brief Adjust root page.
 * @details Pull up the child page if the root page is empty internal page. Or
 * free the root page if it is empty leaf page.
 *
 * @param table_id          table id.
 * @returns                 <code>>0</code> if succesful. <code>0</code>
 * otherwise.
 */
pagenum_t adjust_root(tableid_t table_id);
/**
 * @brief Coalesces two internal pages.
 * @details Moves all right page branch into the left page.
 *
 * @param table_id          table id.
 * @param left_page_idx     left page index.
 * @param seperate_key      key which can seperate between left and right page.
 * @param seperate_key_idx  parent branch index of <code>seperate_key</code>.
 * @param right_page_idx    right page index.
 * @returns                 root page number.
 */
pagenum_t coalesce_internal_nodes(tableid_t table_id, pagenum_t left_page_idx,
                                  recordkey_t seperate_key,
                                  int seperate_key_idx,
                                  pagenum_t right_page_idx);
/**
 * @brief Coalesces two leaf pages.
 * @details Moves all right page record into the left page.
 *
 * @param table_id          table id.
 * @param left_page_idx     left page index.
 * @param right_page_idx    right page index.
 * @returns                 root page number.
 */
pagenum_t coalesce_leaf_nodes(tableid_t table_id, pagenum_t left_page_idx,
                              pagenum_t right_page_idx);
/**
 * @brief Delete a page branch from internal page.
 *
 * @param table_id          table id.
 * @param internal_page_idx internal page index.
 * @param key               branch key.
 * @returns                 root page number.
 */
pagenum_t delete_internal_key(tableid_t table_id, pagenum_t internal_page_idx,
                              recordkey_t key);
/**
 * @brief Delete a record from leaf page.
 *
 * @param table_id          table id.
 * @param leaf_page_idx     leaf page index.
 * @param key               record key.
 * @returns                 root page number.
 */
pagenum_t delete_leaf_key(tableid_t table_id, pagenum_t leaf_page_idx,
                          recordkey_t key);
/**
 * @brief Entrance for remove a record from table.
 *
 * @param table_id          table id.
 * @param key               record key.
 * @returns                 root page number.
 */
pagenum_t delete_node(tableid_t table_id, recordkey_t key);

/**
 * @brief Update a record value.
 *
 * @param       table_id        table id.
 * @param       key             record key.
 * @param       value           new record value.
 * @param       new_val_size    new record value size.
 * @param[out]  old_val_size    old record value size.
 * @param       trx_id          transaction id.
 * @return                      updated record page number.
 */
pagenum_t update_node(tableid_t table_id, recordkey_t key, const char* value,
                      valsize_t new_val_size, valsize_t* old_val_size,
                      trxid_t trx_id);
/** @}*/
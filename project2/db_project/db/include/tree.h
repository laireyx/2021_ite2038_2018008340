#pragma once

#include "types.h"

constexpr int REDISTRIBUTE_THRESHOLD = 2500;
constexpr int MAX_VALUE_SIZE = 1024;

pagenum_t make_leaf(uint64_t parent_page_idx = 0);
pagenum_t create_tree(tableid_t table_id, int64_t key, const char* value,
                      uint16_t value_size);

pagenum_t find_leaf(tableid_t table_id, int64_t key, pagenum_t* leaf_page_idx);
bool find_by_key(tableid_t table_id, int64_t key, char* value = nullptr,
                 uint16_t* value_size = nullptr);

pagenum_t make_node();
pagenum_t make_leaf(void);
pagenum_t insert_into_node(tableid_t table_id, pagenum_t parent_page_idx,
                           pagenum_t left_page_idx, int64_t key,
                           pagenum_t right_page_idx);
pagenum_t insert_into_node_after_splitting(tableid_t table_id,
                                           pagenum_t parent_page_idx, int64_t key,
                                           pagenum_t right_page_idx);
pagenum_t insert_into_parent(tableid_t table_id, pagenum_t left_page_idx, int64_t key,
                             pagenum_t right_page_idx);
pagenum_t insert_into_leaf_after_splitting(tableid_t table_id,
                                           pagenum_t leaf_page_idx, int64_t key,
                                           const char* value,
                                           uint16_t value_size);

pagenum_t insert_node(tableid_t table_id, int64_t key, const char* value,
                      uint16_t value_size);

pagenum_t adjust_root(tableid_t table_id);
pagenum_t coalesce_internal_nodes(tableid_t table_id,
                                  pagenum_t internal_page_idx,
                                  pagenum_t right_page_idx);
pagenum_t coalesce_leaf_nodes(tableid_t table_id, pagenum_t left_page_idx, pagenum_t right_page_idx);
pagenum_t delete_internal_key(tableid_t table_id, pagenum_t internal_page_idx, int64_t key);
pagenum_t delete_leaf_key(tableid_t table_id, pagenum_t leaf_page_idx, int64_t key);
pagenum_t delete_node(tableid_t table_id, int64_t key);
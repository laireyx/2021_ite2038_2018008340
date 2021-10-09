#pragma once

#include "types.h"

pagenum_t make_leaf(uint64_t parent_page_idx = 0);
pagenum_t create_tree(int table_id, int64_t key, const char* value,
                      uint16_t value_size);

pagenum_t find_leaf(int table_id, int64_t key, pagenum_t* leaf_page_idx);
const char* find_by_key(int table_id, int64_t key, uint16_t* value_size);

// record* make_record(int value);
pagenum_t make_node();
pagenum_t make_leaf(void);
pagenum_t insert_into_node(int table_id, pagenum_t parent_page_idx,
                           int left_index, int64_t key,
                           pagenum_t right_page_idx);
pagenum_t insert_into_node_after_splitting(int table_id,
                                           pagenum_t parent_page_idx,
                                           int left_index, int64_t key,
                                           pagenum_t right_page_idx);
pagenum_t insert_into_parent(int table_id, pagenum_t left_page_idx, int64_t key,
                             pagenum_t right_page_idx);
/* pagenum_t insert_into_parent(leafpage_t* left, int64_t key,
                               leafpage_t* right);
pagenum_t insert_into_new_root(leafpage_t* left, int64_t key,
                                 leafpage_t* right); */
pagenum_t insert_into_leaf_after_splitting(int table_id,
                                           pagenum_t leaf_page_idx, int64_t key,
                                           const char* value,
                                           uint16_t value_size);

pagenum_t insert_node(int table_id, int64_t key, const char* value,
                      uint16_t value_size);

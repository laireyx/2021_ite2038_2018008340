#pragma once

#include "types.h"

pagenum_t make_leaf(uint64_t parent_page_idx = 0);
pagenum_t create_tree(int table_id, int64_t key, const char* value,
                      uint16_t value_size);

pagenum_t find_leaf(int table_id, int64_t key, pagenum_t* leaf_page_idx);
char* find_by_key(int table_id, int64_t key, uint16_t* value_size = nullptr);

pagenum_t make_node();
pagenum_t make_leaf(void);
pagenum_t insert_into_node(int table_id, pagenum_t parent_page_idx,
                           int left_index, int64_t key,
                           pagenum_t right_page_idx);
pagenum_t insert_into_node_after_splitting(int table_id,
                                           pagenum_t parent_page_idx, int64_t key,
                                           pagenum_t right_page_idx);
pagenum_t insert_into_parent(int table_id, pagenum_t left_page_idx, int64_t key,
                             pagenum_t right_page_idx);
pagenum_t insert_into_leaf_after_splitting(int table_id,
                                           pagenum_t leaf_page_idx, int64_t key,
                                           const char* value,
                                           uint16_t value_size);

pagenum_t insert_node(int table_id, int64_t key, const char* value,
                      uint16_t value_size);

int get_neighbor_index(pagenum_t n);
pagenum_t adjust_root(pagenum_t root);
pagenum_t coalesce_nodes(pagenum_t root, pagenum_t n, pagenum_t neighbor, int neighbor_index,
                     int k_prime);
pagenum_t redistribute_nodes(pagenum_t root, pagenum_t n, pagenum_t neighbor,
                         int neighbor_index, int k_prime_index, int k_prime);
pagenum_t delete_entry(pagenum_t root, pagenum_t n, int key, void* pointer);
pagenum_t delete_node(int table_id, int64_t key);
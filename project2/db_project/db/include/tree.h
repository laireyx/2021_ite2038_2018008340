#pragma once

#include "types.h"

namespace tree_helper {

leafpage_t* make_leaf(int table_id, int64_t key, const value_t value, uint16_t val_size, uint64_t parent_page_idx = 0);
leafpage_t* create_tree(int table_id, int64_t key, const char* value, uint16_t val_size);

};

std::shared_ptr<leafpage_t> find_leaf(int table_id, int64_t key);
const char* find_by_key(int table_id, int64_t key, uint16_t* val_size);

record* make_record(int value);
leafpage_t* make_node(void);
leafpage_t* make_leaf(void);
int get_left_index(leafpage_t* parent, leafpage_t* left);
leafpage_t* insert_into_leaf(leafpage_t* leaf, int64_t key, record* pointer);
leafpage_t* insert_into_leaf_after_splitting(leafpage_t* leaf, int64_t key,
                                             record* pointer);
leafpage_t* insert_into_node(leafpage_t* parent, int left_index, int64_t key,
                             leafpage_t* right);
leafpage_t* insert_into_node_after_splitting(leafpage_t* parent, int left_index,
                                             int64_t key, leafpage_t* right);
leafpage_t* insert_into_parent(leafpage_t* left, int64_t key,
                               leafpage_t* right);
leafpage_t* insert_into_new_root(leafpage_t* left, int64_t key,
                                 leafpage_t* right);

leafpage_t* insert_node(int table_id, int64_t key, const char* value, uint16_t val_size);

#pragma once

namespace tree_helper {

allocatedpage_t create_tree(const char* value, uint16_t val_size);

};

int find_leaf(int key, bool verbose, leafpage_t* ret_page);
int find_by_key(int key, bool verbose);

record* make_record(int value);
leafpage_t* make_node(void);
leafpage_t* make_leaf(void);
int get_left_index(leafpage_t* parent, leafpage_t* left);
leafpage_t* insert_into_leaf(leafpage_t* leaf, int key, record* pointer);
leafpage_t* insert_into_leaf_after_splitting(leafpage_t* leaf, int key,
                                             record* pointer);
leafpage_t* insert_into_node(leafpage_t* parent, int left_index, int key,
                             leafpage_t* right);
leafpage_t* insert_into_node_after_splitting(leafpage_t* parent, int left_index,
                                             int key, leafpage_t* right);
leafpage_t* insert_into_parent(leafpage_t* left, int key, leafpage_t* right);
leafpage_t* insert_into_new_root(leafpage_t* left, int key, leafpage_t* right);

int insert_node(const char* value, uint16_t val_size, leafpage_t* ret_page);
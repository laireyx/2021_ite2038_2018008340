#include <cstddef>

#include "errors.h"
#include "file.h"
#include "page.h"

<<<<<<< HEAD
namespace tree_helper {

leafpage_t* make_leaf(int table_id, int64_t key, const value_t value, uint16_t val_size, uint64_t parent_page_idx = 0) {
    leafpage_t* leaf_page = new leafpage_t();

    leaf_page->page_header.parent_page_idx = parent_page_idx;
    leaf_page->page_header.is_leaf_page = 0;
    leaf_page->page_header.key_num = 1;

    page_helper::add_leaf_value(leaf_page, key, value, val_size);

    pagenum_t page_idx = file_alloc_page(table_id);
    file_write_page(table_id, page_idx, leaf_page);

    return leaf_page;
}

leafpage_t* create_tree(int table_id, int64_t key, const value_t value, uint16_t val_size) {
    return make_leaf(table_id, key, value, val_size);
}

}

std::shared_ptr<leafpage_t> find_leaf(int table_id, int64_t key) {
=======
leafpage_t* find_leaf(int table_id, int64_t key) {
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

<<<<<<< HEAD
    std::shared_ptr<leafpage_t> ret_page = std::make_shared<leafpage_t>();
    internalpage_t* current_page = reinterpret_cast<internalpage_t*>(ret_page.get());
=======
    leafpage_t* ret_page = new leafpage_t;
    internalpage_t* current_page = reinterpret_cast<internalpage_t*>(ret_page);
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4

    if (!header_page.root_page_idx) {
        return nullptr;
    }

    file_read_page(table_id, header_page.root_page_idx, current_page);
    while (!current_page->page_header.is_leaf_page) {
        int i = 0;
        for (; i < current_page->page_header.key_num; i++) {
            if (key < current_page->page_branches[i].key) break;
        }

        file_read_page(table_id, current_page->page_branches[i].page_idx,
                       current_page);
    }

    return ret_page;
}

const value_t find_by_key(int table_id, int64_t key,
                        uint16_t* val_size = nullptr) {
    int i = 0;
    leafpage_t* leaf_page = find_leaf(table_id, key).get();

    if (!leaf_page) return nullptr;

    PageSlot* leaf_slot = page_helper::get_page_slot(leaf_page);
    uint16_t acc_offset = 0;

    for (i = 0; i < leaf_page->page_header.key_num; i++) {
        if (leaf_slot[i].key == key) break;
        acc_offset += leaf_slot[i].value_offset;
    }

    if (i == leaf_page->page_header.key_num)
        return nullptr;
    else {
        return page_helper::get_leaf_value(leaf_page, i, val_size);
    }
}

<<<<<<< HEAD
leafpage_t* insert_node(int table_id, int64_t key, const value_t value, uint16_t val_size) {
=======
int insert_node(int table_id, int64_t key, const char* value, uint16_t val_size,
                leafpage_t* ret_page) {
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

<<<<<<< HEAD
=======
    // record * pointer;
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4
    // node * leaf;

    /* The current implementation ignores
     * duplicates.
     */

<<<<<<< HEAD
    if (find_by_key(table_id, key) > 0) return nullptr;
=======
    if (find_by_key(table_id, key) > 0) return 0;

    /* Create a new record for the
     * value.
     */
    // pointer = make_record(value);
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4

    /* Case: the tree does not exist yet.
     * Start a new tree.
     */

<<<<<<< HEAD
    if (header_page.root_page_idx == 0)
        return tree_helper::create_tree(table_id, key, value, val_size);
=======
    // if (root == NULL)
    //    return start_new_tree(key, pointer);
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4

    /* Case: the tree already exists.
     * (Rest of function body.)
     */

    // leaf = find_leaf(root, key, false);

    /* Case: leaf has room for key and pointer.
     */

    // if (leaf->num_keys < order - 1) {
    //    leaf = insert_into_leaf(leaf, key, pointer);
    //    return root;
    //}

    /* Case:  leaf must be split.
     */

<<<<<<< HEAD
    return nullptr;
=======
    return 0;
>>>>>>> 715b42a7554ea194e67fe75556e0805216eaeef4
    // return insert_into_leaf_after_splitting(root, leaf, key, pointer);
}
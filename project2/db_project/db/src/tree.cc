#include <cstddef>

#include "errors.h"
#include "file.h"
#include "page.h"

leafpage_t* find_leaf(int table_id, int64_t key) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    leafpage_t* ret_page = new leafpage_t;
    internalpage_t* current_page = reinterpret_cast<internalpage_t*>(ret_page);

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

const char* find_by_key(int table_id, int64_t key,
                        uint16_t* val_size = nullptr) {
    int i = 0;
    leafpage_t* leaf_page = find_leaf(table_id, key);

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

int insert_node(int table_id, int64_t key, const char* value, uint16_t val_size,
                leafpage_t* ret_page) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    // record * pointer;
    // node * leaf;

    /* The current implementation ignores
     * duplicates.
     */

    if (find_by_key(table_id, key) > 0) return 0;

    /* Create a new record for the
     * value.
     */
    // pointer = make_record(value);

    /* Case: the tree does not exist yet.
     * Start a new tree.
     */

    // if (root == NULL)
    //    return start_new_tree(key, pointer);

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

    return 0;
    // return insert_into_leaf_after_splitting(root, leaf, key, pointer);
}
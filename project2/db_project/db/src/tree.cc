#include "tree.h"

#include <cstddef>

#include "errors.h"
#include "file.h"
#include "page.h"

pagenum_t make_node(int table_id, pagenum_t parent_page_idx = 0) {
    allocatedpage_t page = {};
    pagenum_t page_idx = file_alloc_page(table_id);

    page.page_header.is_leaf_page = 0;
    page.page_header.parent_page_idx = parent_page_idx;
    page.page_header.key_num = 0;

    file_write_page(table_id, page_idx, &page);

    return page_idx;
}

pagenum_t make_leaf(int table_id, pagenum_t parent_page_idx = 0) {
    leafpage_t leaf_page;
    pagenum_t leaf_page_idx = make_node(table_id, parent_page_idx);

    file_read_page(table_id, leaf_page_idx, &leaf_page);

    leaf_page.page_header.is_leaf_page = 1;

    file_write_page(table_id, leaf_page_idx, &leaf_page);

    return leaf_page_idx;
}

pagenum_t create_tree(int table_id, int64_t key, const char* value,
                      uint16_t value_size) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    leafpage_t leaf_page;
    pagenum_t leaf_page_idx = make_leaf(table_id);
    file_read_page(table_id, leaf_page_idx, &leaf_page);

    page_helper::add_leaf_value(&leaf_page, key, value, value_size);

    header_page.root_page_idx = leaf_page_idx;

    file_write_page(table_id, leaf_page_idx, &leaf_page);
    file_helper::flush_header(table_id);

    return leaf_page_idx;
}

pagenum_t find_leaf(int table_id, int64_t key) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    internalpage_t current_page;
    pagenum_t current_page_idx;

    if (!header_page.root_page_idx) {
        return 0;
    }

    file_read_page(table_id, header_page.root_page_idx, &current_page);
    while (!current_page.page_header.is_leaf_page) {
        int i = 0;
        for (; i < current_page.page_header.key_num; i++) {
            if (key < current_page.page_branches[i].key) break;
        }

        current_page_idx = current_page.page_branches[i].page_idx;
        file_read_page(table_id, current_page.page_branches[i].page_idx,
                       &current_page);
    }

    return current_page_idx;
}

const char* find_by_key(int table_id, int64_t key,
                        uint16_t* value_size = nullptr) {
    int i = 0;
    leafpage_t leaf_page;
    pagenum_t leaf_page_idx = find_leaf(table_id, key);

    if (!leaf_page_idx) return nullptr;
    file_read_page(table_id, leaf_page_idx, &leaf_page);

    PageSlot* leaf_slot = page_helper::get_page_slot(&leaf_page);

    for (i = 0; i < leaf_page.page_header.key_num; i++) {
        if (leaf_slot[i].key == key) break;
    }

    if (i == leaf_page.page_header.key_num)
        return nullptr;
    else {
        return page_helper::get_leaf_value(&leaf_page, i).get();
    }
}

pagenum_t insert_into_new_root(int table_id, pagenum_t left_page_idx,
                               int64_t key, pagenum_t right_page_idx) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;
    allocatedpage_t left_page, right_page;

    internalpage_t new_root_page;
    pagenum_t new_root_page_idx;

    new_root_page_idx = make_node(table_id);
    file_read_page(table_id, new_root_page_idx, &new_root_page);
    file_read_page(table_id, left_page_idx, &left_page);
    file_read_page(table_id, right_page_idx, &right_page);

    new_root_page.page_branches[0].key = key;

    uint64_t* new_leftmost_child_idx =
        page_helper::get_leftmost_child_idx(&new_root_page);
    *new_leftmost_child_idx = left_page_idx;
    new_root_page.page_branches[0].page_idx = right_page_idx;
    new_root_page.page_header.key_num = 1;
    new_root_page.page_header.parent_page_idx = 0;

    left_page.page_header.parent_page_idx = new_root_page_idx;
    right_page.page_header.parent_page_idx = new_root_page_idx;

    file_write_page(table_id, new_root_page_idx, &new_root_page);
    file_write_page(table_id, left_page_idx, &left_page);
    file_write_page(table_id, right_page_idx, &right_page);

    return new_root_page_idx;
}

pagenum_t insert_into_node(int table_id, pagenum_t parent_page_idx,
                           int left_index, int64_t key,
                           pagenum_t right_page_idx) {
    internalpage_t parent_page;

    file_read_page(table_id, parent_page_idx, &parent_page);

    for (int i = parent_page.page_header.key_num; i > 0 && i > left_index;
         i--) {
        parent_page.page_branches[i] = parent_page.page_branches[i - 1];
    }

    parent_page.page_branches[left_index + 1].key = key;
    parent_page.page_branches[left_index + 1].page_idx = right_page_idx;

    parent_page.page_header.key_num++;

    file_write_page(table_id, parent_page_idx, &parent_page);

    return parent_page_idx;
}

pagenum_t insert_into_node_after_splitting(int table_id, pagenum_t page_idx,
                                           int left_index, int64_t key,
                                           pagenum_t right_page_idx) {
    pagenum_t new_page_idx;
    internalpage_t page, new_page;

    PageBranch* temp_branches = new PageBranch[249];

    file_read_page(table_id, page_idx, &page);

    /* Create the new node and copy
     * half the keys and pointers to the
     * old and half to the new.
     */
    new_page_idx = make_node(table_id);
    file_read_page(table_id, new_page_idx, &new_page);

    for (int i = 0, j = 0; j < 248; i++, j++) {
        if (key > page.page_branches[j].key) {
            temp_branches[i].key = key;
            temp_branches[i].page_idx = right_page_idx;
            i++;
        }
        temp_branches[i] = page.page_branches[j];
    }

    page.page_header.key_num = 0;
    for (int i = 0; i < 124; i++) {
        page_helper::add_internal_key(&page, temp_branches[i].key,
                                      temp_branches[i].page_idx);
    }

    for (int i = 124; i < 248; i++) {
        allocatedpage_t child_page;
        page_helper::add_internal_key(&new_page, temp_branches[i].key,
                                      temp_branches[i].page_idx);

        file_read_page(table_id, temp_branches[i].page_idx, &child_page);

        child_page.page_header.parent_page_idx = new_page_idx;

        file_write_page(table_id, temp_branches[i].page_idx, &child_page);
    }

    /* Insert a new key into the parent of the two
     * nodes resulting from the split, with
     * the old node to the left and the new to the right.
     */

    delete[] temp_branches;

    return insert_into_parent(table_id, page_idx, page.page_branches[123].key,
                              new_page_idx);
}

pagenum_t insert_into_parent(int table_id, pagenum_t left_page_idx, int64_t key,
                             pagenum_t right_page_idx) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;
    allocatedpage_t root_page;
    allocatedpage_t left_page;

    internalpage_t parent_page;
    pagenum_t parent_page_idx;

    file_read_page(table_id, left_page_idx, &left_page);
    file_read_page(table_id, header_page.root_page_idx, &root_page);

    /* Case: new root. */
    if (left_page.page_header.parent_page_idx == 0) {
        return insert_into_new_root(table_id, left_page_idx, key,
                                    right_page_idx);
    }
    file_read_page(table_id, left_page.page_header.parent_page_idx,
                   &parent_page);

    /* Case: leaf or node. (Remainder of
     * function body.)
     */

    /* Find the parent's pointer to the left
     * node.
     */

    int left_index = 0;
    if (*page_helper::get_leftmost_child_idx(&parent_page) == left_page_idx) {
        left_index = -1;
    } else {
        while (left_index < parent_page.page_header.key_num &&
               parent_page.page_branches[left_index].page_idx != left_page_idx)
            left_index++;
    }

    /* Simple case: the new key fits into the node.
     */

    if (parent_page.page_header.key_num < 248)
        return insert_into_node(table_id, parent_page_idx, left_page_idx, key,
                                right_page_idx);

    /* Harder case:  split a node in order
     * to preserve the B+ tree properties.
     */

    return insert_into_node_after_splitting(table_id, parent_page_idx,
                                            left_index, key, right_page_idx);
}

pagenum_t insert_into_leaf_after_splitting(int table_id,
                                           pagenum_t leaf_page_idx, int64_t key,
                                           const char* value,
                                           uint16_t value_size) {
    int64_t new_key;
    leafpage_t leaf_page, new_leaf_page;
    pagenum_t new_leaf_page_idx;
    PageSlot* leaf_slot = page_helper::get_page_slot(&leaf_page);

    PageSlot* temp_slot = new PageSlot[leaf_page.page_header.key_num + 1];
    const char** temp_values =
        new const char*[leaf_page.page_header.key_num + 1];

    file_read_page(table_id, leaf_page_idx, &leaf_page);

    new_leaf_page_idx =
        make_leaf(table_id, leaf_page.page_header.parent_page_idx);
    file_read_page(table_id, new_leaf_page_idx, &new_leaf_page);

    // Make a temporary page slot.
    for (int i = 0, j = 0, offset = 0; j < leaf_page.page_header.key_num;
         i++, j++) {
        if (i == j && key > leaf_slot[j].key) {
            temp_slot[i].key = key;
            temp_slot[i].value_size = value_size;
            temp_values[i] = value;
            i++;
        }
        temp_slot[i] = leaf_slot[j];
        temp_values[i] = page_helper::get_leaf_value(&leaf_page, j).get();
    }

    int split_start = 0;
    int acc_len = 0;
    for (; split_start < leaf_page.page_header.key_num + 1; split_start++) {
        acc_len += temp_slot[split_start].value_size;
        if (acc_len >= 1984) {
            break;
        }
    }

    leaf_page.page_header.key_num = 0;
    *page_helper::get_free_space(&leaf_page) = 0;
    for (int i = 0; i < split_start; i++) {
        page_helper::add_leaf_value(&leaf_page, temp_slot[i].key,
                                    temp_values[i], temp_slot[i].value_size);
    }

    new_key = temp_slot[split_start].key;
    for (int i = split_start; i < leaf_page.page_header.key_num; i++) {
        page_helper::add_leaf_value(&new_leaf_page, temp_slot[i].key,
                                    temp_values[i], value_size);
    }

    uint64_t* leaf_sibling_idx = page_helper::get_sibling_idx(&leaf_page);
    uint64_t* new_leaf_sibling_idx =
        page_helper::get_sibling_idx(&new_leaf_page);

    *new_leaf_sibling_idx = *leaf_sibling_idx;
    *leaf_sibling_idx = new_leaf_page_idx;

    file_write_page(table_id, leaf_page_idx, &leaf_page);
    file_write_page(table_id, new_leaf_page_idx, &new_leaf_page);

    delete[] temp_slot;
    delete[] temp_values;

    return insert_into_parent(table_id, leaf_page_idx, new_key,
                              new_leaf_page_idx);
}

pagenum_t insert_node(int table_id, int64_t key, const char* value,
                      uint16_t value_size) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    leafpage_t leaf_page;
    pagenum_t leaf_page_idx;

    /* The current implementation ignores
     * duplicates.
     */

    if (find_by_key(table_id, key) != nullptr) return 0;

    /* Case: the tree does not exist yet.
     * Start a new tree.
     */

    if (header_page.root_page_idx == 0)
        return create_tree(table_id, key, value, value_size);

    /* Case: the tree already exists.
     * (Rest of function body.)
     */

    leaf_page_idx = find_leaf(table_id, key);
    file_read_page(table_id, leaf_page_idx, &leaf_page);

    /* Case: leaf has room for key and pointer.
     */

    if (page_helper::has_enough_space(&leaf_page, value_size)) {
        page_helper::add_leaf_value(&leaf_page, key, value, value_size);
        return leaf_page_idx;
    }

    /* Case:  leaf must be split.
     */

    return insert_into_leaf_after_splitting(table_id, leaf_page_idx, key, value,
                                            value_size);
}
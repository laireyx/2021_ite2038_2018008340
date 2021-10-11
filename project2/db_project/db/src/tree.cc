#include "tree.h"

#include <vector>
#include <utility>
#include <algorithm>

#include "errors.h"
#include "file.h"
#include "page.h"

pagenum_t make_node(tableid_t table_id, pagenum_t parent_page_idx = 0) {
    allocatedpage_t page = {};
    pagenum_t page_idx = file_alloc_page(table_id);

    page.page_header.is_leaf_page = 0;
    page.page_header.parent_page_idx = parent_page_idx;
    page.page_header.key_num = 0;

    file_write_page(table_id, page_idx, &page);

    return page_idx;
}

pagenum_t make_leaf(tableid_t table_id, pagenum_t parent_page_idx = 0) {
    leafpage_t leaf_page;
    pagenum_t leaf_page_idx = make_node(table_id, parent_page_idx);

    file_read_page(table_id, leaf_page_idx, &leaf_page);

    leaf_page.page_header.is_leaf_page = 1;
    leaf_page.page_header.reserved_footer.footer_1 = 3968;
    leaf_page.page_header.reserved_footer.footer_2 = 0;

    file_write_page(table_id, leaf_page_idx, &leaf_page);

    return leaf_page_idx;
}

pagenum_t create_tree(tableid_t table_id, int64_t key, const char* value,
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

pagenum_t find_leaf(tableid_t table_id, int64_t key) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    internalpage_t current_page;
    pagenum_t current_page_idx = header_page.root_page_idx;

    if (!current_page_idx) {
        return 0;
    }

    file_read_page(table_id, header_page.root_page_idx, &current_page);
    while (!current_page.page_header.is_leaf_page) {
        int i = 0;
        for (i = 0; i < current_page.page_header.key_num; i++) {
            if (key < current_page.page_branches[i].key) break;
        }
        i--;

        if(i >= 0) {
            current_page_idx = current_page.page_branches[i].page_idx;
        } else {
            current_page_idx = *page_helper::get_leftmost_child_idx(reinterpret_cast<internalpage_t*>(&current_page));
        }
        file_read_page(table_id, current_page_idx, &current_page);
    }

    return current_page_idx;
}

char* find_by_key(tableid_t table_id, int64_t key,
                        uint16_t* value_size) {
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
        return page_helper::get_leaf_value(&leaf_page, i, value_size);
    }
}

pagenum_t insert_into_new_root(tableid_t table_id, pagenum_t left_page_idx,
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

    header_page.root_page_idx = new_root_page_idx;
    file_helper::flush_header(table_id);

    return new_root_page_idx;
}

pagenum_t insert_into_node(tableid_t table_id, pagenum_t parent_page_idx,
                           pagenum_t left_page_idx, int64_t key,
                           pagenum_t right_page_idx) {
    internalpage_t parent_page;

    file_read_page(table_id, parent_page_idx, &parent_page);

    page_helper::add_internal_key(&parent_page, key, right_page_idx);
    std::sort(parent_page.page_branches,
        parent_page.page_branches + parent_page.page_header.key_num,
        [](const PageBranch& a, const PageBranch& b) {
            return a.key < b.key;
        });
    /// @todo use this.
    /*int i = parent_page.page_header.key_num;
    for (; i >= 0; i--) {
        if(i == 0 || parent_page.page_branches[i - 1].page_idx == left_page_idx) {
            parent_page.page_branches[i].key = key;
            parent_page.page_branches[i].page_idx = right_page_idx;
            break;
        }
        parent_page.page_branches[i] = parent_page.page_branches[i - 1];
    }*/

    file_write_page(table_id, parent_page_idx, &parent_page);

    return parent_page_idx;
}

pagenum_t insert_into_node_after_splitting(tableid_t table_id, pagenum_t page_idx, int64_t key,
                                           pagenum_t right_page_idx) {
    pagenum_t new_page_idx;
    internalpage_t page, new_page;

    std::vector<PageBranch> temp_branches;

    file_read_page(table_id, page_idx, &page);

    /* Create the new node and copy
     * half the keys and pointers to the
     * old and half to the new.
     */
    new_page_idx = make_node(table_id);
    file_read_page(table_id, new_page_idx, &new_page);

    for (int i = 0; i < 248; i++) {
        temp_branches.push_back(page.page_branches[i]);
    }
    PageBranch new_branch;

    new_branch.key = key;
    new_branch.page_idx = right_page_idx;
    temp_branches.push_back(new_branch);
    std::sort(temp_branches.begin(), temp_branches.end(), [](const PageBranch& a, const PageBranch& b) {
        return a.key < b.key;
    });

    page.page_header.key_num = 0;
    for (int i = 0; i < 124; i++) {
        page_helper::add_internal_key(&page, temp_branches[i].key,
                                      temp_branches[i].page_idx);
    }

    for (int i = 124; i < 249; i++) {
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

    file_write_page(table_id, page_idx, &page);
    file_write_page(table_id, new_page_idx, &new_page);

    return insert_into_parent(table_id, page_idx, new_page.page_branches[0].key,
                              new_page_idx);
}

pagenum_t insert_into_parent(tableid_t table_id, pagenum_t left_page_idx, int64_t key,
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

    parent_page_idx = left_page.page_header.parent_page_idx;
    file_read_page(table_id, parent_page_idx,
                   &parent_page);

    /* Case: leaf or node. (Remainder of
     * function body.)
     */

    /* Find the parent's pointer to the left
     * node.
     */

    /* Simple case: the new key fits into the node.
     */

    if (parent_page.page_header.key_num < 248)
        return insert_into_node(table_id, parent_page_idx, left_page_idx, key,
                                right_page_idx);

    /* Harder case:  split a node in order
     * to preserve the B+ tree properties.
     */

    return insert_into_node_after_splitting(table_id, parent_page_idx, key, right_page_idx);
}

pagenum_t insert_into_leaf_after_splitting(tableid_t table_id,
                                           pagenum_t leaf_page_idx, int64_t key,
                                           const char* value,
                                           uint16_t value_size) {
    int64_t new_key;
    leafpage_t leaf_page, new_leaf_page;
    pagenum_t new_leaf_page_idx;
    PageSlot* leaf_slot = page_helper::get_page_slot(&leaf_page);

    file_read_page(table_id, leaf_page_idx, &leaf_page);

    int total_values_num = leaf_page.page_header.key_num + 1;
    std::vector<std::pair<PageSlot, const char*>> temp;

    new_leaf_page_idx =
        make_leaf(table_id, leaf_page.page_header.parent_page_idx);
    file_read_page(table_id, new_leaf_page_idx, &new_leaf_page);

    // Make a temporary page slot.
    for (int i = 0; i < leaf_page.page_header.key_num; i++) {
        temp.push_back(std::make_pair(
            leaf_slot[i],
            page_helper::get_leaf_value(&leaf_page, i)
        ));
    }
    PageSlot new_slot;
    char* value_copy = new char[value_size];
    memcpy(value_copy, value, value_size);

    new_slot.key = key;
    new_slot.value_size = value_size;

    temp.push_back(std::make_pair(
        new_slot,
        value_copy
    ));

    std::sort(temp.begin(), temp.end(), [](const std::pair<PageSlot, const char*>& a, const std::pair<PageSlot, const char*>& b) {
        return a.first.key < b.first.key;
    });

    int split_start = 0;
    int acc_len = 0;
    for (; split_start < leaf_page.page_header.key_num + 1; split_start++) {
        acc_len += temp[split_start].first.value_size;
        if (acc_len >= (PAGE_SIZE - PAGE_HEADER_SIZE) / 2) {
            break;
        }
    }

    leaf_page.page_header.key_num = 0;
    *page_helper::get_free_space(&leaf_page) = PAGE_SIZE - PAGE_HEADER_SIZE;

    for (int i = 0; i < split_start; i++) {
        page_helper::add_leaf_value(&leaf_page, temp[i].first.key,
                                    temp[i].second, temp[i].first.value_size);
    }

    new_key = temp[split_start].first.key;
    for (int i = split_start; i < total_values_num; i++) {
        page_helper::add_leaf_value(&new_leaf_page, temp[i].first.key,
                                    temp[i].second, value_size);
    }

    uint64_t* leaf_sibling_idx = page_helper::get_sibling_idx(&leaf_page);
    uint64_t* new_leaf_sibling_idx =
        page_helper::get_sibling_idx(&new_leaf_page);

    *new_leaf_sibling_idx = *leaf_sibling_idx;
    *leaf_sibling_idx = new_leaf_page_idx;

    file_write_page(table_id, leaf_page_idx, &leaf_page);
    file_write_page(table_id, new_leaf_page_idx, &new_leaf_page);

    for(auto& temp_pair : temp) {
        delete[] temp_pair.second;
    }

    return insert_into_parent(table_id, leaf_page_idx, new_key,
                              new_leaf_page_idx);
}

pagenum_t insert_node(tableid_t table_id, int64_t key, const char* value,
                      uint16_t value_size) {
    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    leafpage_t leaf_page;
    pagenum_t leaf_page_idx;

    /* The current implementation ignores
     * duplicates.
     */

    char* existing_check;

    if ((existing_check = find_by_key(table_id, key)) != nullptr) {
        delete[] existing_check;
        return 0;
    }

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
        std::vector<std::pair<PageSlot, const char*>> temp;
        PageSlot* leaf_slot = page_helper::get_page_slot(&leaf_page);

        for (int i = 0; i < leaf_page.page_header.key_num; i++) {
            temp.push_back(std::make_pair(
                leaf_slot[i],
                page_helper::get_leaf_value(&leaf_page, i)
            ));
        }

        PageSlot new_slot;
        char* value_copy = new char[value_size];
        memcpy(value_copy, value, value_size);

        new_slot.key = key;
        new_slot.value_size = value_size;

        temp.push_back(std::make_pair(
            new_slot,
            value_copy
        ));

        std::sort(temp.begin(), temp.end(), [](const std::pair<PageSlot, const char*>& a, const std::pair<PageSlot, const char*>& b) {
            return a.first.key < b.first.key;
        });

        leaf_page.page_header.key_num = 0;
        *page_helper::get_free_space(&leaf_page) = PAGE_SIZE - PAGE_HEADER_SIZE;

        for(auto& temp_pair : temp) {
            page_helper::add_leaf_value(&leaf_page, temp_pair.first.key, temp_pair.second, temp_pair.first.value_size);
        }

        file_write_page(table_id, leaf_page_idx, &leaf_page);

        for(auto& temp_pair : temp) {
            delete[] temp_pair.second;
        }
        return leaf_page_idx;
    }

    /* Case:  leaf must be split.
     */

    return insert_into_leaf_after_splitting(table_id, leaf_page_idx, key, value,
                                            value_size);
}

pagenum_t coalesce_leaf_nodes(tableid_t table_id, pagenum_t left_page_idx, pagenum_t right_page_idx) {

    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    int64_t old_key;
    internalpage_t parent_page;
    leafpage_t left_page, right_page;
    PageSlot* right_slot;

    file_read_page(table_id, left_page_idx, &left_page);
    file_read_page(table_id, right_page_idx, &right_page);
    file_read_page(table_id, left_page.page_header.parent_page_idx, &parent_page);

    /* In a leaf, append the keys and pointers of
     * n to the neighbor.
     * Set the neighbor's last pointer to point to
     * what had been n's right neighbor.
     */

    right_slot = page_helper::get_page_slot(&right_page);

    for (int i = 0; i < right_page.page_header.key_num; i++) {
        char* right_slot_value = page_helper::get_leaf_value(&right_page, i);
        page_helper::add_leaf_value(&left_page, right_slot[i].key, right_slot_value, right_slot[i].value_size);
        delete[] right_slot_value;
    }
    *page_helper::get_sibling_idx(&left_page) = *page_helper::get_sibling_idx(&right_page);

    for(int i = 0; i < parent_page.page_header.key_num; i++) {
        if(parent_page.page_branches[i].page_idx == right_page_idx) {
            delete_internal_key(table_id, left_page.page_header.parent_page_idx, parent_page.page_branches[i].key);
            break;
        }
    }

    file_write_page(table_id, left_page_idx, &left_page);
    file_free_page(table_id, right_page_idx);

    return header_page.root_page_idx;
}

pagenum_t delete_internal_key(tableid_t table_id, pagenum_t internal_page_idx, int64_t key) {

    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    int capacity;

    pagenum_t sibling_page_idx;
    leafpage_t internal_page, sibling_page;

    pagenum_t parent_page_idx;
    internalpage_t parent_page;

    // Remove key and pointer from node.

    file_read_page(table_id, internal_page_idx, &internal_page);

    /* Case:  deletion from the root. 
     */

    if (internal_page_idx == header_page.root_page_idx)
        return adjust_root(header_page.root_page_idx);


    /* Case:  deletion from a node below the root.
     * (Rest of function body.)
     */

    /* Determine minimum allowable size of node,
     * to be preserved after deletion.
     */

    /* Case:  node stays at or above minimum.
     * (The simple case.)
     */

    if (*page_helper::get_free_space(&leaf_page) <= (PAGE_SIZE - PAGE_HEADER_SIZE) / 2)
        return header_page.root_page_idx;

    /* Case:  node falls below minimum.
     * Either coalescence or redistribution
     * is needed.
     */

    /* Find the appropriate neighbor node with which
     * to coalesce.
     * Also find the key (k_prime) in the parent
     * between the pointer to node n and the pointer
     * to the neighbor.
     */

    sibling_page_idx = *page_helper::get_sibling_idx(&leaf_page);
    if(sibling_page_idx == 0) {
        sibling_page_idx = parent_page.page_branches[parent_page.page_header.key_num - 2].page_idx;
    }
    file_read_page(table_id, sibling_page_idx, &sibling_page);

    /* Coalescence. */

    if (*page_helper::get_free_space(&leaf_page) + *page_helper::get_free_space(&sibling_page) >= PAGE_SIZE - PAGE_HEADER_SIZE) {
        if(*page_helper::get_sibling_idx(&leaf_page) == 0)
            return coalesce_nodes(table_id, parent_page.page_branches[parent_page.page_header.key_num - 2].page_idx, leaf_page_idx);
        else
            return coalesce_nodes(table_id, leaf_page_idx, sibling_page_idx);
    }

    /* Redistribution. */

    else
        return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}

pagenum_t delete_leaf_key(tableid_t table_id, pagenum_t leaf_page_idx, int64_t key) {

    auto& instance = file_helper::get_table(table_id);

    headerpage_t& header_page = instance.header_page;

    int capacity;

    pagenum_t sibling_page_idx;
    leafpage_t leaf_page, sibling_page;

    pagenum_t parent_page_idx;
    internalpage_t parent_page;
    file_read_page(table_id, leaf_page_idx, &leaf_page);

    parent_page_idx = leaf_page.page_header.parent_page_idx;
    file_read_page(table_id, parent_page_idx, &parent_page);

    page_helper::remove_leaf_value(&leaf_page, key);

    if (leaf_page_idx == header_page.root_page_idx)
        return adjust_root(header_page.root_page_idx);

    if (*page_helper::get_free_space(&leaf_page) <= (PAGE_SIZE - PAGE_HEADER_SIZE) / 2)
        return leaf_page_idx;

    sibling_page_idx = *page_helper::get_sibling_idx(&leaf_page);
    if(sibling_page_idx == 0) {
        sibling_page_idx = parent_page.page_branches[parent_page.page_header.key_num - 2].page_idx;
    }
    file_read_page(table_id, sibling_page_idx, &sibling_page);

    if (*page_helper::get_free_space(&leaf_page) + *page_helper::get_free_space(&sibling_page) >= PAGE_SIZE - PAGE_HEADER_SIZE) {
        if(*page_helper::get_sibling_idx(&leaf_page) == 0)
            return coalesce_leaf_nodes(table_id, parent_page.page_branches[parent_page.page_header.key_num - 2].page_idx, leaf_page_idx);
        else
            return coalesce_leaf_nodes(table_id, leaf_page_idx, sibling_page_idx);
    }

    else
        return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}

pagenum_t delete_node(tableid_t table_id, int64_t key) {
    pagenum_t leaf_page_idx = find_leaf(table_id, key);

    if(find_by_key(table_id, key) && leaf_page_idx) {
        return delete_leaf_key(table_id, leaf_page_idx, key);
        //free(key_record);
    }

    return 0;
}
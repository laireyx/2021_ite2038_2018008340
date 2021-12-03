#include <page.h>
#include <types.h>

#include <cstring>
#include <iostream>
#include <vector>

namespace page_helper {
PageSlot* get_page_slot(LeafPage* page) {
    return reinterpret_cast<PageSlot*>(page->reserved);
}

void get_leaf_value(LeafPage* page, int value_idx, char* value,
                    valsize_t* value_size) {
    PageSlot* leaf_slot = get_page_slot(page);

    if (value_size) *value_size = leaf_slot[value_idx].value_size;

    get_leaf_value(page, leaf_slot[value_idx].value_offset,
                   leaf_slot[value_idx].value_size, value);
}

void get_leaf_value(LeafPage* page, uint16_t value_offset, valsize_t value_size,
                    char* value) {
    if (value)
        memcpy(value, reinterpret_cast<uint8_t*>(page) + value_offset,
               value_size);
}

bool has_enough_space(LeafPage* page, valsize_t value_size) {
    uint64_t free_space_amount = page->page_header.reserved_footer.footer_1;
    return free_space_amount >= value_size + sizeof(PageSlot);
}

uint64_t* get_free_space(LeafPage* page) {
    return &(page->page_header.reserved_footer.footer_1);
}

uint64_t* get_sibling_idx(LeafPage* page) {
    return &(page->page_header.reserved_footer.footer_2);
}

bool add_leaf_value(LeafPage* page, recordkey_t key, const char* value,
                    valsize_t value_size) {
    if (!has_enough_space(page, value_size)) {
        return false;
    }

    uint64_t* free_space_amount = get_free_space(page);
    PageSlot* leaf_slot = get_page_slot(page);

    uint16_t value_offset = PAGE_SIZE - value_size;

    leaf_slot[page->page_header.key_num].key = key;
    leaf_slot[page->page_header.key_num].value_size = value_size;
    if (page->page_header.key_num > 0) {
        value_offset =
            leaf_slot[page->page_header.key_num - 1].value_offset - value_size;
    }
    leaf_slot[page->page_header.key_num].value_offset = value_offset;

    memcpy(reinterpret_cast<uint8_t*>(page) + value_offset, value, value_size);

    page->page_header.key_num++;
    *free_space_amount -= value_size + sizeof(PageSlot);

    return true;
}

bool remove_leaf_value(LeafPage* page, recordkey_t key) {
    PageSlot* leaf_slot = get_page_slot(page);

    uint16_t start_offset;
    uint16_t offset_shift;

    for (int i = 0; i < page->page_header.key_num; i++) {
        if (leaf_slot[i].key == key) {
            start_offset = leaf_slot[i].value_offset;
            offset_shift = leaf_slot[i].value_size;
            for (int j = i + 1; j < page->page_header.key_num; j++) {
                memmove(reinterpret_cast<uint8_t*>(page) +
                            leaf_slot[j].value_offset + offset_shift,
                        reinterpret_cast<uint8_t*>(page) +
                            leaf_slot[j].value_offset,
                        leaf_slot[j].value_size);

                leaf_slot[j - 1].key = leaf_slot[j].key;
                leaf_slot[j - 1].value_offset =
                    leaf_slot[j].value_offset + offset_shift;
                leaf_slot[j - 1].value_size = leaf_slot[j].value_size;
            }
            page->page_header.key_num--;
            *get_free_space(page) += offset_shift + sizeof(PageSlot);
            return true;
        }
    }

    return false;
}

bool set_leaf_value(LeafPage* page, recordkey_t key, char* old_value, valsize_t* old_val_size,
                    const char* new_value, valsize_t new_val_size) {
    PageSlot* leaf_slot = get_page_slot(page);

    uint16_t start_offset;
    uint16_t offset_shift;

    for (int i = 0; i < page->page_header.key_num; i++) {
        if (leaf_slot[i].key == key) {
            /// @todo update at here
            if (old_val_size != nullptr)
                *old_val_size = leaf_slot[i].value_size;
            if (old_value != nullptr)
                memcpy(old_value,
                       reinterpret_cast<uint8_t*>(page) +
                           leaf_slot[i].value_offset,
                       leaf_slot[i].value_size);

            leaf_slot[i].value_size = new_val_size;
            memcpy(reinterpret_cast<uint8_t*>(page) + leaf_slot[i].value_offset,
                   new_value, new_val_size);
            return true;
        }
    }

    return false;
}

bool add_internal_key(InternalPage* page, recordkey_t key, pagenum_t page_idx) {
    if (page->page_header.key_num == 248) {
        return false;
    }

    page->page_branches[page->page_header.key_num].key = key;
    page->page_branches[page->page_header.key_num].page_idx = page_idx;

    page->page_header.key_num++;

    return true;
}

bool remove_internal_key(InternalPage* page, recordkey_t key) {
    if (page->page_header.key_num == 0) {
        return false;
    }

    for (int i = 0; i < page->page_header.key_num; i++) {
        if (page->page_branches[i].key == key) {
            memmove(page->page_branches + i, page->page_branches + i + 1,
                    sizeof(PageBranch) * (page->page_header.key_num - 1 - i));
            break;
        }
    }

    page->page_header.key_num--;

    return true;
}

uint64_t* get_leftmost_child_idx(InternalPage* page) {
    return &(page->page_header.reserved_footer.footer_2);
}

}  // namespace page_helper
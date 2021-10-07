#include "page.h"
#include "types.h"

#include <memory>

#include <string.h>

namespace page_helper {
PageSlot* get_page_slot(LeafPage* page) {
    return reinterpret_cast<PageSlot*>(page->reserved);
}

const value_t get_leaf_value(LeafPage* page, int value_idx,
                           uint16_t* value_size = nullptr) {
    PageSlot* leaf_slot = get_page_slot(page);

    if (value_size) *value_size = leaf_slot[value_idx].value_size;

    return get_leaf_value(page, leaf_slot[value_idx].value_offset,
                          leaf_slot[value_idx].value_size);
}

const value_t get_leaf_value(LeafPage* page, uint16_t value_offset,
                           uint16_t value_size) {
    value_t ret_val(new char[value_size]);

    memcpy(ret_val.get(),
           page->reserved + (PAGE_SIZE - PAGE_HEADER_SIZE) -
               (value_offset - value_size),
           value_size);

    return ret_val;
}

bool is_leaf_full(LeafPage* page, uint16_t value_size) {
    uint16_t* free_space_amount = reinterpret_cast<uint16_t*>(page->page_header.reserved + PAGE_HEADER_SIZE - 16 - 16);
    return (*free_space_amount) < value_size + sizeof(PageSlot);
}

bool add_leaf_value(LeafPage* page, int64_t key, const value_t value, uint16_t value_size) {
    if(is_leaf_full(page, value_size)) {
        return false;
    }
    
    uint16_t* free_space_amount = reinterpret_cast<uint16_t*>(page->page_header.reserved + PAGE_HEADER_SIZE - 16 - 16);
    PageSlot* leaf_slot = get_page_slot(page);

    uint16_t value_offset = 0;

    leaf_slot[page->page_header.key_num].key = key;
    leaf_slot[page->page_header.key_num].value_size = value_size;
    if(page->page_header.key_num > 0) {
        value_offset = leaf_slot[page->page_header.key_num - 1].value_offset + value_size;
    }
    leaf_slot[page->page_header.key_num].value_offset = value_offset;

    memcpy(page->reserved + (PAGE_SIZE - PAGE_HEADER_SIZE) - (value_offset - value_size),
           value.get(),
           value_size);

    page->page_header.key_num++;
    *free_space_amount -= value_size + sizeof(PageSlot);

    return true;
}

}  // namespace page_helper
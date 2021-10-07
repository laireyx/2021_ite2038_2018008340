#include "page.h"

#include <string.h>

namespace page_helper {
PageSlot* get_page_slot(LeafPage* page) {
    return reinterpret_cast<PageSlot*>(page->reserved);
}

const char* get_leaf_value(LeafPage* page, int value_idx,
                           uint16_t* val_size = nullptr) {
    PageSlot* leaf_slot = get_page_slot(page);

    if (val_size) *val_size = leaf_slot[value_idx].value_size;

    return get_leaf_value(page, leaf_slot[value_idx].value_offset,
                          leaf_slot[value_idx].value_size);
}

const char* get_leaf_value(LeafPage* page, uint16_t value_offset,
                           uint16_t value_size) {
    char* ret_val = new char[value_size];

    memcpy(ret_val,
           page->reserved + (PAGE_SIZE - PAGE_HEADER_SIZE) -
               (value_offset - value_size),
           value_size);

    return ret_val;
}
}  // namespace page_helper
#include "page.h"
#include <cstddef>

namespace page_helper {
PageSlot* get_page_slot(LeafPage* page) {
    return reinterpret_cast<PageSlot*>(page->reserved);
}
}
/**
 * @addtogroup DiskSpaceManager
 * @{
 */
#pragma once
#include "types.h"

#include <cstdint>

/// @brief  Size of each page(in bytes).
constexpr int PAGE_SIZE = 4096;

/// @brief  Size of page header(in bytes).
constexpr int PAGE_HEADER_SIZE = 128;

/// @brief  Maximum number of page branches.
constexpr int MAX_PAGE_BRANCHES = 248;

/**
 * @class   Page
 * @brief   struct for abstract page.
 * @details Actually this struct is empty for equalizing the size of all
 *          inherited pages.
 */
struct Page {};

/**
 * @class   HeaderPage
 * @brief   struct for the header page.
 */
struct HeaderPage : public Page {
    /// @brief The first free page index.
    pagenum_t free_page_idx;
    /// @brief Total count of the page reserved.
    uint64_t page_num;
    /// @brief The root page index.
    pagenum_t root_page_idx;

    /// @property Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 24];
};

/**
 * @class   FreePage
 * @brief   struct for the free page.
 */
struct FreePage : public Page {
    /// @brief Index of the very next free page.
    pagenum_t next_free_idx;

    /// @brief Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 8];
};

/**
 * @class   PageHeader
 * @brief   page header for allocated(internal and leaf) node.
 */
struct PageHeader {
    /// @brief The parent page index.
    pagenum_t parent_page_idx;
    /// @brief 1 if this page is leaf page, and 0 if internal page.
    uint32_t is_leaf_page;
    /// @brief Number of keys this page is holding.
    uint32_t key_num;

    /// @brief Reserved area for page header.
    uint8_t reserved[PAGE_HEADER_SIZE - 16 - 16];

    struct ReservedFooter {
        /// @brief Can be free space(in leaf page).
        uint64_t footer_1;
        /// @brief Can be leftmost children idx(in internal page) or next
        /// sibling idx(in leaf page).
        uint64_t footer_2;
    } reserved_footer;
};

/**
 * @class   PageSlot
 * @brief   page slot for allocated(internal and leaf) node.
 */
struct PageSlot {
    /// @brief The page key.
    int64_t key;
    /// @brief The value size(in bytes).
    uint16_t value_size;
    /// @brief The value offset(in bytes).
    uint16_t value_offset;
} __attribute__((packed));

/**
 * @class   PageBranch
 * @brief   page slot for internal node.
 */
struct PageBranch {
    /// @brief The page key.
    int64_t key;
    /// @brief The page index.
    uint16_t page_idx;
};

/**
 * @class   AllocatedPage
 * @brief   struct for allocated page.
 */
struct AllocatedPage : public Page {
    /// @brief Page header.
    PageHeader page_header;
};

/**
 * @class   AllocatedFullPage
 * @brief   struct for any allocated page.
 * @details You should use this struct for read & write allocated page.
 */
struct AllocatedFullPage : public AllocatedPage {
    /// @brief Reserved area for normal allocated page.
    uint8_t reserved[PAGE_SIZE - PAGE_HEADER_SIZE];
};

/**
 * @class   InternalPage
 * @brief   struct for allocated internal page.
 */
struct InternalPage : public AllocatedPage {

    /// @brief Page branches.
    PageBranch page_branches[MAX_PAGE_BRANCHES];
};

/**
 * @class   LeafPage
 * @brief   struct for allocated leaf page.
 */
struct LeafPage : public AllocatedPage {

    /// @brief Reserved area for normal allocated page.
    uint8_t reserved[PAGE_SIZE - PAGE_HEADER_SIZE];
} __attribute__((packed));

namespace page_helper {

PageSlot* get_page_slot(LeafPage* page);
void get_leaf_value(LeafPage* page, int value_idx, char* value,
                    uint16_t* value_size = nullptr);
void get_leaf_value(LeafPage* page, uint16_t value_offset, uint16_t value_size,
                    char* value);

bool has_enough_space(LeafPage* page, uint16_t value_size);
uint64_t* get_free_space(LeafPage* page);
pagenum_t* get_sibling_idx(LeafPage* page);

bool add_leaf_value(LeafPage* page, int64_t key, const char* value,
                    uint16_t value_size);
bool remove_leaf_value(LeafPage* page, int64_t key);

bool set_internal_key(InternalPage* page, int position, int64_t key,
                      pagenum_t page_idx);
bool add_internal_key(InternalPage* page, int64_t key, pagenum_t page_idx);
bool remove_internal_key(InternalPage* page, int64_t key);
pagenum_t* get_leftmost_child_idx(InternalPage* page);
}

typedef Page page_t;
typedef PageHeader pageheader_t;
typedef HeaderPage headerpage_t;
typedef FreePage freepage_t;
typedef AllocatedFullPage allocatedpage_t;
typedef InternalPage internalpage_t;
typedef LeafPage leafpage_t;

/** @}*/
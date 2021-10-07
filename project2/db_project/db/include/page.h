/**
 * @addtogroup DiskSpaceManager
 * @{
 */
#pragma once
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
    uint64_t free_page_idx;
    /// @brief Total count of the page reserved.
    uint64_t page_num;
    /// @brief The root page index.
    uint64_t root_page_idx;

    /// @property Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 24];
};

/**
 * @class   FreePage
 * @brief   struct for the free page.
 */
struct FreePage : public Page {
    /// @brief Index of the very next free page.
    uint64_t next_free_idx;

    /// @brief Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 8];
};

/**
 * @class   PageHeader
 * @brief   page header for allocated(internal and leaf) node.
 */
struct PageHeader {
    /// @brief The parent page index.
    uint64_t parent_page_idx;
    /// @brief 1 if this page is leaf page, and 0 if internal page.
    uint32_t is_leaf_page;
    /// @brief Number of keys this page is holding.
    uint32_t key_num;

    /// @brief Reserved area for page header.
    uint8_t reserved[PAGE_HEADER_SIZE - 16];
};

/**
 * @class   PageSlot
 * @brief   page slot for allocated(internal and leaf) node.
 */
struct PageSlot {
    /// @brief The page key.
    uint64_t key;
    /// @brief The value size(in bytes).
    uint16_t value_size;
    /// @brief The value offset(in bytes).
    uint16_t value_offset;
};

/**
 * @class   PageBranch
 * @brief   page slot for internal node.
 */
struct PageBranch {
    /// @brief The page key.
    uint64_t key;
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
 * @class   InternalPage
 * @brief   struct for allocated internal page.
 */
struct InternalPage : public AllocatedPage {
    /// @brief Reserved area for normal allocated page header.
    /// uint8_t reserved_header[8];
    /// @brief Additional page index for leftmost children.
    /// uint64_t leftmost_children_idx;

    /// @brief Page branches.
    PageBranch page_branches[MAX_PAGE_BRANCHES];
};

/**
 * @class   LeafPage
 * @brief   struct for allocated leaf page.
 */
struct LeafPage : public AllocatedPage {
    /// @brief Page header.
    PageHeader page_header;
    /// @brief Amount of free space in this leaf page.
    /// uint64_t free_space;
    /// @brief The right sibling page index.
    /// uint64_t next_sibling_idx;

    /// @brief Reserved area for normal allocated page.
    uint8_t reserved[PAGE_SIZE - PAGE_HEADER_SIZE];
};

namespace page_helper {
PageSlot* get_page_slot(LeafPage* page);
}

/** @}*/
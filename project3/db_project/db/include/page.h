/**
 * @addtogroup DiskSpaceManager
 * @{
 */
#pragma once
#include <types.h>

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
 * @class   FullPage
 * @brief   struct for any page.
 */
struct FullPage : public Page {
    /// @brief Reserved area for page.
    uint8_t reserved[PAGE_SIZE];
};

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

    /// @brief Reserved area for next project.
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

/**
 * @brief   Page helper
 * @details This namespace includes some helper functions which are used by
 * APIs such as tree manager which uses direct page access. These functions do not call any other APIs,
 * but modifying only given data.
 */
namespace page_helper {

/**
 * @brief Get page slots.
 * @details It just return the reserved area as <code>PageSlot*</code>, which
 * means it does not give any hints for the number of slots. Anyway, you can
 * still get the number of the slots by using
 * <code>page->page_header.key_num</code>
 *
 * @param page  leaf page.
 * @returns <code>PageSlot*</code>
 */
PageSlot* get_page_slot(LeafPage* page);
/**
 * @brief Get leaf value.
 * @details Get a leaf value corresponds to index <code>value_idx</code>, using
 * the page slot information. Then copies the value and its size into given
 * pointer, which is given by caller.
 *
 * @param page              leaf page.
 * @param value_idx         index number of value.
 * @param[out] value        value will be set into this pointer if not null.
 * @param[out] value_size   value size will be set into this pointer if not
 * null.
 */
void get_leaf_value(LeafPage* page, int value_idx, char* value,
                    uint16_t* value_size = nullptr);
/**
 * @brief Get leaf value.
 * @details Get a leaf value using exact offset and size. Usually it does not
 * called from the outside, and just help the other function. Then copies the
 * value and its size into given pointer, which is given by caller.
 *
 * @param page          leaf page.
 * @param value_offset  value offset.
 * @param value_size    value size.
 * @param[out] value    value will be set into this pointer if not null.
 */
void get_leaf_value(LeafPage* page, uint16_t value_offset, uint16_t value_size,
                    char* value);
/**
 * @brief Check if given page has enough space for given value size.
 * @details Required size for the given <code>value_size</code> is
 * <code>value_size + sizeof(PageSlot)</code>.
 *
 * @param page          leaf page.
 * @param value_size    value size.
 * @returns             <code>true</code> if space is enough, <code>false</code>
 * otherwise.
 */
bool has_enough_space(LeafPage* page, uint16_t value_size);
/**
 * @brief Get free space amount.
 * @details In leaf page, <code>page->page_header.reserved_footer.footer_1</code> represents current free space amount.
 * 
 * @param page          Leaf page.
 * @returns The pointer to the free space amount, which will be more useful than raw value in purpose like modifying.
 */
uint64_t* get_free_space(LeafPage* page);
/**
 * @brief Get next sibling index.
 * @details In leaf page,
 * <code>page->page_header.reserved_footer.footer_2</code> represents the very
 * next(it means right) sibling index.
 *
 * @returns right sibling index if page is not rightmost child. <code>0</code>
 * if it is.
 */
pagenum_t* get_sibling_idx(LeafPage* page);

/**
 * @brief Add a leaf value into the last position of the leaf page.
 *
 * @param page          leaf page.
 * @param key           record key.
 * @param value         record value.
 * @param value_size    record value size.
 * @returns             <code>true</code> if the page has enough space and
 * appending is successful, <code>false</code> otherwise.
 */
bool add_leaf_value(LeafPage* page, int64_t key, const char* value,
                    uint16_t value_size);
/**
 * @brief Remove a record and compact reserved area in the leaf page.
 *
 * @param page          leaf page.
 * @param key           record key.
 * @returns             <code>true</code> if the key was inside the leaf record
 * and deleted successfully, <code>false</code> otherwise.
 */
bool remove_leaf_value(LeafPage* page, int64_t key);

/**
 * @brief Add a page branch into the last position of the internal page.
 *
 * @param page          internal page.
 * @param key           branch key.
 * @param page_idx      branch page index.
 * @returns             <code>true</code> if the page has enough space and
 * appending is successful, <code>false</code> otherwise.
 */
bool add_internal_key(InternalPage* page, int64_t key, pagenum_t page_idx);
/**
 * @brief Remove a page branch and realign branches.
 *
 * @param page          internal page.
 * @param key           branch key.
 * @returns             <code>true</code> if the key was inside the internal
 * branch and deleted successfully, <code>false</code> otherwise.
 */
bool remove_internal_key(InternalPage* page, int64_t key);
/**
 * @brief Get leftmost child page index.
 * @details In internal page,
 * <code>page->page_header.reserved_footer.footer_2</code> represents current
 * free space amount.
 *
 * @param page          internal page.
 * @returns             leftmost child page index.
 */
pagenum_t* get_leftmost_child_idx(InternalPage* page);
}

typedef Page page_t;
typedef FullPage fullpage_t;
typedef PageHeader pageheader_t;
typedef HeaderPage headerpage_t;
typedef FreePage freepage_t;
typedef AllocatedFullPage allocatedpage_t;
typedef InternalPage internalpage_t;
typedef LeafPage leafpage_t;

/** @}*/
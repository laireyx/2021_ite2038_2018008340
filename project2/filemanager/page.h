#pragma once
#include <cstdint>

constexpr int PAGE_SIZE = 4096;

/*!
 * @class Page struct for abstract page.
 */
struct page_t { };

/*!
 * @class Page struct for allocated page.
 */
struct allocatedpage_t : public page_t {
    //! @property Reserved area for normal allocated page.
    uint8_t reserved[PAGE_SIZE];
};

/*!
 * @class Page struct for the header page.
 * @extends page_t
 *
 */
struct headerpage_t : public page_t {
    //! @property The first free page index
    uint64_t free_page_idx;
    //! @property Total count of the page reserved.
    uint64_t page_num;

    //! @property Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 16];
};

struct freepage_t : public page_t {
    //! @property Index of the very next free page.
    uint64_t next_free_idx;

    //! @property Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 8];
};
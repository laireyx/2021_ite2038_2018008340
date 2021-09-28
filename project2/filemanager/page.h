#pragma once
#include <cstdint>

/// @brief  Size of each page(in bytes).
constexpr int PAGE_SIZE = 4096;

/**
 * @class   Page
 * @brief   struct for abstract page.
 * @details Actually this struct is empty for equalizing the size of all inherited pages.
 */
struct Page { };

/**
 * @class   AllocatedPage
 * @brief   struct for allocated page.
 */
struct AllocatedPage : public Page {
    /// @brief Reserved area for normal allocated page.
    uint8_t reserved[PAGE_SIZE];
};

/**
 * @class   HeaderPage
 * @brief   struct for the header page.
 */
struct HeaderPage : public Page {
    /// @brief The first free page index
    uint64_t free_page_idx;
    /// @brief Total count of the page reserved.
    uint64_t page_num;

    /// @property Reserved area for next project.
    uint8_t reserved[PAGE_SIZE - 16];
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
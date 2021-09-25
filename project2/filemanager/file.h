#pragma once

#include "types.h"

/**
 * @brief Allocate an on-disk page from the free page list
 *
 * @return The very free page index.
 */
pagenum_t file_alloc_page();

/**
 * @brief Free an on-disk page to the free page list
 *
 * @param pagenum page index.
 */
void file_free_page(pagenum_t pagenum);

/**
 * @brief Read an on-disk page into the in-memory page structure(dest)
 *
 * @param pagenum page index.
 * @param dest the pointer of the page data.
 */
void file_read_page(pagenum_t pagenum, page_t* dest);

/**
 * @brief Write an in-memory page(src) to the on-disk page
 *
 * @param pagenum page index.
 * @param src the pointer of the page data.
 */
void file_write_page(pagenum_t pagenum, const page_t* src);
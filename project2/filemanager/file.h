#pragma once

#include "types.h"

/// @brief Maximum number of database instances count.
constexpr int MAX_DATABASE_INSTANCE = 1024;

/**
 * @class DatabaseInstance
 * @brief Database file instance.
 */
typedef struct DatabaseInstance {
    /// @brief Database file path.
    char* file_path;
    /// @brief Database file pointer.
    FILE* file_pointer;
} DatabaseInstance;

/*!
 * @brief Seek page file pointer at offset matching with given page index.
 *
 * @param pagenum page index.
 */
void _seek_page(pagenum_t pagenum);

/*!
 * @brief Automatically check and size-up a page file.
 * @details Extend capacity if newsize if specified. Or if there are no space for the next free page, double the reserved page count.
 *
 * @param newsize extended size. default is 0, which means doubleing the reserved page count if there are no free page.
 */
void _extend_capacity(pagenum_t newsize);

/*!
 * @brief Flush a header page as "pagenum 0".
 */
void _flush_header();

/**
 * @brief Open existing database file or create one if not existed.
 *
 * @param path Database file path.
 * @return ID of the opened database file.
 */
int64_t file_open_database_file(const char* path);

/**
 * @brief Allocate an on-disk page from the free page list
 *
 * @return Allocated page index.
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

/**
 * @brief Stop referencing the database file
 */
void file_close_database_file();
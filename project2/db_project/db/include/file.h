/**
 * @addtogroup DiskSpaceManager
 * @{
 */
#pragma once

#include "types.h"

/// @brief      Initial size(in bytes) of newly created database file.
/// @details    It means 10MiB.
constexpr int INITIAL_DB_FILE_SIZE = 10 * 1024 * 1024;

/// @brief      Maximum number of database instances count.
constexpr int MAX_DATABASE_INSTANCE = 1024;

/// @brief      Initial number of page count in newly created database file.
/// @details    Its value is 2560.
constexpr int INITIAL_DATABASE_CAPS =
    INITIAL_DB_FILE_SIZE / MAX_DATABASE_INSTANCE;

/**
 * @brief   Filemanager helper
 * @details This namespace includes some helper functions which are used by
 * filemanager API. Those functions are not a part of filemanager API, but
 * frequently used part of it so I wrapped these functions.
 */
namespace file_helper {
/**
 * @brief   Switch current database into given database.
 * @details If current <code>database_fd == fd</code>, then do nothing.
 *          If not, change database_fd to given fd and re-read header_page from
 * it.
 *
 * @param fd Database file descriptor obtained with
 * <code>file_open_database_file()</code>.
 */
bool switch_to_fd(int fd);

/**
 * @brief   Automatically check and size-up a page file.
 * @details If <code>newsize > page_num</code>, reserve pages so that total page
 * num is equivalent to newsize. If <code>newsize = 0</code> and header page's
 * free_page_idx is 0, double the reserved page count.
 *
 * @param   newsize extended size. default is 0, which means doubling the
 * reserved page count if there are no free page.
 */
void extend_capacity(pagenum_t newsize);

/**
 * @brief   Flush a header page as "pagenum 0".
 * @details Write header page into offset 0 of the current database file
 * descriptor.
 */
void flush_header();
};  // namespace file_helper

/**
 * @brief   Open existing database file or create one if not existed.
 *
 * @param   path    Database file path.
 * @return          ID of the opened database file.
 */
int file_open_database_file(const char* path);

/**
 * @brief   Allocate an on-disk page from the free page list
 *
 * @param   fd  Database file descriptor obtained with
 *              <code>file_open_database_file()</code>.
 * @return  >0  Page index number if allocation success.
 *          0   Zero if allocation failed.
 */
pagenum_t file_alloc_page(int fd);

/**
 * @brief   Free an on-disk page to the free page list
 *
 * @param   fd      Database file descriptor obtained with
 * <code>file_open_database_file()</code>.
 * @param   pagenum page index.
 */
void file_free_page(int fd, pagenum_t pagenum);

/**
 * @brief   Read an on-disk page into the in-memory page structure(dest)
 *
 * @param   fd      Database file descriptor obtained with
 * <code>file_open_database_file()</code>.
 * @param   pagenum page index.
 * @param   dest    the pointer of the page data.
 */
void file_read_page(int fd, pagenum_t pagenum, page_t* dest);

/**
 * @brief   Write an in-memory page(src) to the on-disk page
 *
 * @param   fd      Database file descriptor obtained with
 * <code>file_open_database_file()</code>.
 * @param   pagenum page index.
 * @param   src     the pointer of the page data.
 */
void file_write_page(int fd, pagenum_t pagenum, const page_t* src);

/**
 * @brief   Stop referencing the database file
 */
void file_close_database_file();
/** @}*/
/**
 * @addtogroup BufferManager
 * @{
 */
#pragma once

#include <page.h>
#include <types.h>

/**
 * @brief   Allocate an on-disk page from the free page list
 *
 * @param   table_id        table id obtained with
 *                          <code>file_open_table_file()</code>.
 * @return  >0  Page index number if allocation success.
 *          0   Zero if allocation failed.
 */
pagenum_t buffered_alloc_page(int64_t table_id);

/**
 * @brief   Free an on-disk page to the free page list
 *
 * @param   table_id        table id obtained with
 *                          <code>file_open_table_file()</code>.
 * @param   pagenum         page index.
 */
void buffered_free_page(int64_t table_id, pagenum_t pagenum);

/**
 * @brief   Read an on-disk page into the in-memory page structure(dest)
 *
 * @param   table_id        table id obtained with
 *                          <code>file_open_table_file()</code>.
 * @param   pagenum         page index.
 * @param   dest            the pointer of the page data.
 */
void buffered_read_page(int64_t table_id, pagenum_t pagenum, page_t* dest);

/**
 * @brief   Write an in-memory page(src) to the on-disk page
 *
 * @param   table_id        table id obtained with
 *                          <code>file_open_table_file()</code>.
 * @param   pagenum         page index.
 * @param   src             the pointer of the page data.
 */
void buffered_write_page(int64_t table_id, pagenum_t pagenum, const page_t* src);

/** @}*/
/**
 * @addtogroup BufferManager
 * @{
 */
#pragma once

#include <page.h>
#include <types.h>

#include <functional>

typedef std::pair<tableid_t, pagenum_t> PageLocation;
namespace std {
template <>
struct hash<PageLocation> {
    size_t operator()(const PageLocation& location) const {
        size_t hash_value = 17;
        hash_value = hash_value * 31 + std::hash<tableid_t>()(location.first);
        hash_value = hash_value * 31 + std::hash<tableid_t>()(location.second);
        return hash_value;
    }
};
}  // namespace std

typedef struct BufferBlock {
    /// @brief buffered page.
    fullpage_t page;
    /// @brief page location.
    PageLocation page_location;
    /// @brief <code>true</code> if this buffer has been modified,
    /// <code>false</code> otherwise.
    bool is_dirty;
    /// @brief <code>true</code> if this buffer is currently using,
    /// <code>false</code> otherwise.
    int is_pinned;

    /// @brief revious buffer block index of Recently-Used linked list.
    /// <code>-1</code> if this buffer is the first item.
    int prev_block_idx;
    /// @brief next buffer block index of Recently-Used linked list.
    /// <code>-1</code> if this buffer is the last item.
    int next_block_idx;
} BufferBlock;

/**
 * @brief   BufferManager helper
 * @details This namespace includes some helper functions which are used by
 * buffermanager API. Those functions are not a part of buffermanager API, but
 * frequently used part of it so I wrapped these functions.
 */
namespace buffer_helper {
/**
 * @brief   Load a page into buffer.
 * @details Return a buffer block if exists. If not, automatically evict a
 * buffer with the lowest priority and load a buffer in that position. If there
 * are no room for more buffer, fallback direct I/O method will be used.
 *
 * @param       page_location   page location.
 * @param[out]  page            page.
 * @param       pin             pin.
 * @return loaded buffer.
 */
BufferBlock* load_buffer(const PageLocation& page_location, page_t* page,
                         bool pin = true);
/**
 * @brief   Apply a page into buffer.
 * @details Apply page content into buffer block if exists. If not, return
 * <code>false</code> to notify fallback direct I/O method should be used.
 *
 * @param page_location page location.
 * @param page          page content.
 * @returns <code>true</code> if buffer write success, <code>false</code> if
 * fallback method is used.
 */
bool apply_buffer(const PageLocation& page_location, const page_t* page);
/**
 * @brief Check if there buffer slot is full.
 *
 * @return true if there are no room
 */
bool is_full();
/**
 * @brief Evict a buffer with the lowest priority.
 * @details <code>load_buffer()</code> will determine using of fallback method
 * with return value of <code>evict()</code>.
 *
 * @return evicted buffer slot index if success, <code><0</code> otherwise.
 */
int evict();
}  // namespace buffer_helper

/**
 * @brief   Initialize buffer manager.
 *
 * @param   buffer_size     Buffer size.
 * @return  <code>0</code> if success, non-zero value otherwise.
 */
int init_buffer(int buffer_size);

/**
 * @brief   Open existing table file or create one if not existed.
 *
 * @param   path    Table file path.
 * @return          ID of the opened table file.
 */
tableid_t buffered_open_table_file(const char* path);

/**
 * @brief   Allocate an on-disk page from the free page list
 *
 * @param   table_id        table id obtained with
 *                          <code>buffered_open_table_file()</code>.
 * @return  >0  Page index number if allocation success.
 *          0   Zero if allocation failed.
 */
pagenum_t buffered_alloc_page(tableid_t table_id);

/**
 * @brief   Free an on-disk page to the free page list
 *
 * @param   table_id        table id obtained with
 *                          <code>buffered_open_table_file()</code>.
 * @param   pagenum         page index.
 */
void buffered_free_page(tableid_t table_id, pagenum_t pagenum);

/**
 * @brief   Read an on-disk page into the in-memory page structure(dest)
 *
 * @param   table_id        table id obtained with
 *                          <code>buffered_open_table_file()</code>.
 * @param   pagenum         page index.
 * @param   dest            the pointer of the page data.
 * @param   pin             <code>true</code> if this buffer will be writed
 * after.
 */
void buffered_read_page(tableid_t table_id, pagenum_t pagenum, page_t* dest,
                        bool pin = true);

/**
 * @brief   Write an in-memory page(src) to the on-disk page
 *
 * @param   table_id        table id obtained with
 *                          <code>buffered_open_table_file()</code>.
 * @param   pagenum         page index.
 * @param   src             the pointer of the page data.
 */
void buffered_write_page(tableid_t table_id, pagenum_t pagenum,
                         const page_t* src);

/**
 * @brief   Shutdown buffer manager.
 *
 * @return  <code>0</code> if success, non-zero value otherwise.
 */
int shutdown_buffer();

/** @}*/
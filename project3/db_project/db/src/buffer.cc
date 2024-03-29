/**
 * @addtogroup BufferManager
 * @{
 */
#include <buffer.h>
#include <errors.h>
#include <file.h>

#include <cstring>
#include <new>
#include <unordered_map>
#include <utility>
#include <iostream>

/// @brief buffer slot.
BufferBlock* buffer_slot = nullptr;
/// @brief head index of Recently-Used list.
int buffer_head_idx = -1;
/// @brief tail index of Recently-Used list.
int buffer_tail_idx = -1;
/// @brief size of buffer block.
int buffer_size = 0;

std::unordered_map<PageLocation, int> buffer_index;

namespace buffer_helper {
BufferBlock* load_buffer(tableid_t table_id, pagenum_t pagenum, page_t* page,
                         bool pin) {
    auto page_location = std::make_pair(table_id, pagenum);
    const auto& existing_buffer = buffer_index.find(page_location);
    if (existing_buffer != buffer_index.end()) {
        int buffer_page_idx = existing_buffer->second;
        BufferBlock* buffer_page = buffer_slot + buffer_page_idx;

        detach_from_tree(buffer_page_idx);
        prepend_to_head(buffer_page_idx);

        buffer_page->is_pinned = (pin ? 1 : 0);

        if (page != nullptr) {
            memcpy(page, &(buffer_page->page), PAGE_SIZE);
        }
        return buffer_slot + buffer_page_idx;
    }

    int evicted_idx = evict();
    if (evicted_idx > 0) {
        // add
        BufferBlock* buffer_page = buffer_slot + evicted_idx;
        BufferBlock* buffer_head = buffer_slot + buffer_head_idx;

        detach_from_tree(evicted_idx);
        buffer_tail_idx = buffer_page->prev_block_idx;
        prepend_to_head(evicted_idx);

        buffer_index[page_location] = evicted_idx;

        buffer_page->is_dirty = false;
        buffer_page->is_pinned = pin ? 1 : 0;
        buffer_page->page_location = page_location;

        file_read_page(table_id, pagenum, &(buffer_page->page));
        if (page != nullptr) {
            memcpy(page, &(buffer_page->page), PAGE_SIZE);
        }
        return buffer_page;
    }

    // direct I/O fallback
    if (page != nullptr) {
        file_read_page(table_id, pagenum, page);
    }
    return nullptr;
}

bool apply_buffer(tableid_t table_id, pagenum_t pagenum, const page_t* page) {
    auto page_location = std::make_pair(table_id, pagenum);
    const auto& existing_buffer = buffer_index.find(page_location);
    if (existing_buffer != buffer_index.end()) {
        int buffer_page_idx = existing_buffer->second;
        BufferBlock* buffer_page = buffer_slot + buffer_page_idx;

        detach_from_tree(buffer_page_idx);
        prepend_to_head(buffer_page_idx);

        memcpy(&(buffer_page->page), page, PAGE_SIZE);
        buffer_page->is_dirty = true;
        buffer_page->is_pinned = 0;
        return true;
    }

    // direct I/O fallback
    file_write_page(table_id, pagenum, page);
    return false;
}

void release_buffer(tableid_t table_id, pagenum_t pagenum) {
    auto page_location = std::make_pair(table_id, pagenum);
    const auto& existing_buffer = buffer_index.find(page_location);
    if (existing_buffer != buffer_index.end()) {
        int buffer_page_idx = existing_buffer->second;
        BufferBlock* buffer_page = buffer_slot + buffer_page_idx;
        buffer_page->is_pinned = 0;
    }
}

int evict() {
    int evicted_idx = buffer_tail_idx;
    BufferBlock* buffer_evict = buffer_slot + evicted_idx;

    while (evicted_idx != -1 && buffer_evict->is_pinned) {
        evicted_idx = buffer_evict->prev_block_idx;
        buffer_evict = buffer_slot + evicted_idx;
    }

    // All the buffers are using
    if (evicted_idx == -1) {
        return -1;
    }

    buffer_index.erase(buffer_evict->page_location);

    // Flush to file if dirty
    if (buffer_evict->is_dirty) {
        tableid_t table_id;
        pagenum_t page_num;
        std::tie(table_id, page_num) = buffer_evict->page_location;
        file_write_page(table_id, page_num, &buffer_evict->page);
    }

    return evicted_idx;
}

void detach_from_tree(int buffer_idx) {
    BufferBlock* buffer_page = buffer_slot + buffer_idx;

    if (buffer_page->prev_block_idx != -1) {
        BufferBlock* buffer_prev = buffer_slot + (buffer_page->prev_block_idx);
        buffer_prev->next_block_idx = buffer_page->next_block_idx;
    }

    if (buffer_page->next_block_idx != -1) {
        BufferBlock* buffer_next = buffer_slot + (buffer_page->next_block_idx);
        buffer_next->prev_block_idx = buffer_page->prev_block_idx;
    }
}

void prepend_to_head(int buffer_idx) {
    BufferBlock* buffer_page = buffer_slot + buffer_idx;
    BufferBlock* buffer_head = buffer_slot + buffer_head_idx;

    buffer_page->prev_block_idx = -1;
    buffer_page->next_block_idx = buffer_head_idx;
    buffer_head->prev_block_idx = buffer_idx;

    buffer_head_idx = buffer_idx;
}
}  // namespace buffer_helper

int init_buffer(int _buffer_size) {
    try {
        if (buffer_slot != nullptr) {
            if (_buffer_size == buffer_size) {
                return 0;
            } else {
                return -1;
            }
        }
        buffer_size = _buffer_size;
        buffer_slot = new BufferBlock[buffer_size];
        for (int i = 0; i < buffer_size; i++) {
            buffer_slot[i].page_location = std::make_pair(MAX_TABLE_INSTANCE + 1, 0);
            buffer_slot[i].is_dirty = false;
            buffer_slot[i].is_pinned = 0;

            if (i < buffer_size - 1) buffer_slot[i].next_block_idx = i + 1;
            if (i > 0) buffer_slot[i].prev_block_idx = i - 1;
        }

        buffer_slot[0].prev_block_idx = -1;
        buffer_slot[buffer_size - 1].next_block_idx = -1;

        buffer_head_idx = 0;
        buffer_tail_idx = buffer_size - 1;

        return 0;
    } catch (const std::bad_alloc& err) {
        return -1;
    }
}

tableid_t buffered_open_table_file(const char* path) {
    return file_open_table_file(path);
}

pagenum_t buffered_alloc_page(tableid_t table_id) {
    file_helper::extend_capacity(table_id);

    headerpage_t header_page;

    buffer_helper::load_buffer(table_id, 0, &header_page);

    // Pop the first page from free page stack.
    pagenum_t free_page_idx = header_page.free_page_idx;

    freepage_t allocated_page;
    buffer_helper::load_buffer(table_id, free_page_idx, &allocated_page);

    // Move the first free page index to the next page.
    header_page.free_page_idx = allocated_page.next_free_idx;

    buffer_helper::apply_buffer(table_id, 0, &header_page);
    buffer_helper::apply_buffer(table_id, free_page_idx, &allocated_page);

    return free_page_idx;
}

void buffered_free_page(tableid_t table_id, pagenum_t pagenum) {
    headerpage_t header_page;
    buffer_helper::load_buffer(table_id, 0, &header_page);

    // Current first free page index
    pagenum_t old_free_page_idx = header_page.free_page_idx;
    // Newly freed page
    freepage_t new_free_page;

    // Next free page index of newly freed page is current first free page
    // index. Its just pushing the pagenum into free page stack.
    new_free_page.next_free_idx = old_free_page_idx;
    buffer_helper::apply_buffer(table_id, pagenum, &new_free_page);

    // Set the first free page to freed page number.
    header_page.free_page_idx = pagenum;
    buffer_helper::apply_buffer(table_id, 0, &header_page);
}

void buffered_read_page(tableid_t table_id, pagenum_t pagenum, page_t* dest,
                        bool pin) {
    buffer_helper::load_buffer(table_id, pagenum, dest, pin);
}

void buffered_write_page(tableid_t table_id, pagenum_t pagenum,
                         const page_t* src) {
    buffer_helper::apply_buffer(table_id, pagenum, src);
}

void buffered_release_page(tableid_t table_id, pagenum_t pagenum) {
    buffer_helper::release_buffer(table_id, pagenum);
}

int shutdown_buffer() {
    if (buffer_slot != nullptr) {
        for (int i = 0; i < buffer_size; i++) {
            tableid_t table_id;
            pagenum_t page_num;

            BufferBlock* buffer = buffer_slot + i;
            std::tie(table_id, page_num) = buffer->page_location;

            if (buffer->is_dirty) {
                file_write_page(table_id, page_num, &buffer->page);
            }
        }
        delete[] buffer_slot;
        buffer_slot = nullptr;
        buffer_size = 0;
        buffer_index.clear();
    }
    file_close_table_files();
    return 0;
}
/** @}*/
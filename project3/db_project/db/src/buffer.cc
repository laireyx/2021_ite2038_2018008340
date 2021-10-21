/**
 * @addtogroup BufferManager
 * @{
 */
#include <buffer.h>

pagenum_t buffered_alloc_page(int64_t table_id) {
    return 0;
}

void buffered_free_page(int64_t table_id, pagenum_t pagenum) {
    return;
}

void buffered_read_page(int64_t table_id, pagenum_t pagenum, page_t* dest) {
    return;
}

void buffered_write_page(int64_t table_id, pagenum_t pagenum, const page_t* src) {
    return;
}
/** @}*/
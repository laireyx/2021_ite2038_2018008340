
#include <cstdio>
#include "file.h"
#include "preferences.h"

bool initialized = false;
FILE* page_file;
headerpage_t header_page;

/*!
 * @brief Initialize file manager.
 * @details Open a page file and read header page, or create and initialize if not exists.
 */
void _initialize() {
	if ((page_file = fopen(FILE_NAME, "r+b")) == NULL) {
		page_file = fopen(FILE_NAME, "w+b");

		header_page.free_page_idx = 0;
		header_page.page_num = 1;

		fwrite(&header_page, PAGE_SIZE, 1, page_file);
		fflush(page_file);
	}
	else {
		fread(&header_page, PAGE_SIZE, 1, page_file);
	}
	initialized = true;
}

/*!
 * @brief Automatically check and size-up a page file.
 * @details If there are no space for the next free page, double the reserved page count.
 */
void _page_growth() {
	if (header_page.free_page_idx == 0) {
		for (
			pagenum_t free_page_index = header_page.page_num;
			free_page_index < header_page.page_num * 2;
			free_page_index++
			) {

			freepage_t free_page;

			if (free_page_index < header_page.page_num * 2 - 1)
				free_page.next_free_idx = free_page_index + 1;
			else
				free_page.next_free_idx = 0;

			file_write_page(free_page_index, reinterpret_cast<page_t*>(&free_page));
		}

		header_page.free_page_idx = header_page.page_num;
		header_page.page_num *= 2;
	}
}

/*!
 * @brief Seek page file pointer at offset matching with given page index.
 *
 * @param pagenum page index.
 */
void _seek_page(pagenum_t pagenum) {
	fseek(page_file, pagenum * PAGE_SIZE, SEEK_SET);
}

/*!
 * @brief Flush a header page as "pagenum 0".
 */
void _flush_header() {
	fseek(page_file, 0, SEEK_SET);
	fwrite(&header_page, PAGE_SIZE, 1, page_file);
	fflush(page_file);
}

pagenum_t file_alloc_page() {
	if (!initialized) _initialize();

	_page_growth();

	pagenum_t free_page_idx = header_page.free_page_idx;
	freepage_t free_page;

	file_read_page(free_page_idx, reinterpret_cast<page_t*>(&free_page));
	header_page.free_page_idx = free_page.next_free_idx;

	_flush_header();

	return free_page_idx;
}

void file_free_page(pagenum_t pagenum) {
	pagenum_t old_free_page_idx = header_page.free_page_idx;
	freepage_t new_free_page;

	new_free_page.next_free_idx = old_free_page_idx;
	file_write_page(pagenum, reinterpret_cast<page_t*>(&new_free_page));
	header_page.free_page_idx = pagenum;

	_flush_header();

	return;
}

void file_read_page(pagenum_t pagenum, page_t* dest) {
	if (!initialized) _initialize();
	_seek_page(pagenum);
	fread(dest, PAGE_SIZE, 1, page_file);
}

void file_write_page(pagenum_t pagenum, const page_t* src) {
	if (!initialized) _initialize();
	_seek_page(pagenum);
	fwrite(src, PAGE_SIZE, 1, page_file);
	fflush(page_file);
}
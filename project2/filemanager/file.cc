
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "file.h"

/// @brief current database instance number
int database_instance_count = 0;
/// @brief all database instances
DatabaseInstance database_instances[MAX_DATABASE_INSTANCE + 1];

/// @brief currently opened database file pointer
FILE* database_file = nullptr;
/// @brief currently opened database header page
headerpage_t header_page;

/*!
 * @brief Automatically check and size-up a page file.
 * @details Extend capacity if newsize if specified. Or if there are no space for the next free page, double the reserved page count.
 *
 * @param newsize extended size. default is 0, which means doubleing the reserved page count if there are no free page.
 */
void _extend_capacity(pagenum_t newsize = 0) {
	if (
		newsize > header_page.page_num ||
		header_page.free_page_idx == 0
	) {
		if (newsize == 0) {
			newsize = header_page.page_num * 2;
		}

		for (
			pagenum_t free_page_index = header_page.page_num;
			free_page_index < newsize;
			free_page_index++
		) {

			freepage_t free_page;

			if (free_page_index < newsize - 1)
				free_page.next_free_idx = free_page_index + 1;
			else
				free_page.next_free_idx = 0;

			file_write_page(free_page_index, reinterpret_cast<page_t*>(&free_page));
		}

		header_page.free_page_idx = header_page.page_num;
		header_page.page_num = newsize;
	}
}

/*!
 * @brief Seek page file pointer at offset matching with given page index.
 *
 * @param pagenum page index.
 */
void _seek_page(pagenum_t pagenum) {
	fseek(database_file, pagenum * PAGE_SIZE, SEEK_SET);
}

/*!
 * @brief Flush a header page as "pagenum 0".
 */
void _flush_header() {
	fseek(database_file, 0, SEEK_SET);
	fwrite(&header_page, PAGE_SIZE, 1, database_file);
	fflush(database_file);
}

int64_t file_open_database_file(const char* path) {
	for (
		int index = 1;
		index <= database_instance_count;
		index++
	) {
		if (
			strcmp(
				database_instances[index].file_path,
				path
			) == 0
		) {
			return index;
		}
	}

	if (database_instance_count >= MAX_DATABASE_INSTANCE) {
		return -1;
	}

	DatabaseInstance& new_instance = database_instances[++database_instance_count];
	new_instance.file_path = reinterpret_cast<char*>(malloc(sizeof(char) * (strlen(path) + 1)));
	strncpy(new_instance.file_path, path, strlen(path) + 1);

	if ((database_file = fopen(path, "r+b")) == nullptr) {
		database_file = fopen(path, "w+b");

		header_page.free_page_idx = 0;
		header_page.page_num = 1;

		_extend_capacity(2560);

		_flush_header();
	}
	else {
		fread(&header_page, PAGE_SIZE, 1, database_file);
	}

	new_instance.file_pointer = database_file;
	return database_instance_count;
}

pagenum_t file_alloc_page() {
	_extend_capacity();

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
	_seek_page(pagenum);
	fread(dest, PAGE_SIZE, 1, database_file);
}

void file_write_page(pagenum_t pagenum, const page_t* src) {
	_seek_page(pagenum);
	fwrite(src, PAGE_SIZE, 1, database_file);
	fflush(database_file);
}

void file_close_database_file() {
	for (
		int index = 1;
		index <= database_instance_count;
		index++
	) {
		fclose(database_instances[index].file_pointer);
	}
}
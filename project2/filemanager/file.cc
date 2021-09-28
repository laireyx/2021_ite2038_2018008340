
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <cassert>
#include "file.h"

/// @brief current database instance number
int database_instance_count = 0;
/// @brief all database instances
DatabaseInstance database_instances[MAX_DATABASE_INSTANCE];

/// @brief currently opened database file descriptor
int database_fd = 0;

/// @brief currently opened database header page
headerpage_t header_page;

void _switch_to_fd(int fd) {
	assert(fd != 0);
	if(database_fd == fd) return;

	database_fd = fd;
	pread64(database_fd, &header_page, PAGE_SIZE, 0);
}

void _extend_capacity(pagenum_t newsize = 0) {
	if (
		newsize > header_page.page_num ||
		header_page.free_page_idx == 0
	) {
		if (newsize == 0) {
			newsize = header_page.page_num * 2;
		}

		for (
			pagenum_t free_page_idx = header_page.page_num;
			free_page_idx < newsize;
			free_page_idx++
		) {

			freepage_t free_page;

			if (free_page_idx < newsize - 1)
				free_page.next_free_idx = free_page_idx + 1;
			else
				free_page.next_free_idx = 0;

			pwrite64(database_fd, &free_page, PAGE_SIZE, free_page_idx * PAGE_SIZE);
		}

		header_page.free_page_idx = header_page.page_num;
		header_page.page_num = newsize;
	}
}

void _flush_header() {
	assert(database_fd != 0);
	pwrite64(database_fd, &header_page, PAGE_SIZE, 0);
}

int file_open_database_file(const char* path) {
	for (
		int index = 0;
		index < database_instance_count;
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

	DatabaseInstance& new_instance = database_instances[database_instance_count++];
	new_instance.file_path = new char[strlen(path) + 1];
	strncpy(new_instance.file_path, path, strlen(path) + 1);

	if ((database_fd = open(path, O_RDWR | O_SYNC | O_CREAT | O_EXCL, 0644)) > 0) {
		header_page.free_page_idx = 0;
		header_page.page_num = 1;

		_extend_capacity(2560);

		_flush_header();
	}
	else {
		database_fd = open(path, O_RDWR | O_SYNC);
		read(database_fd, &header_page, PAGE_SIZE);
	}

	return new_instance.file_descriptor = database_fd;
}

pagenum_t file_alloc_page(int fd) {
	_switch_to_fd(fd);
	_extend_capacity();

	pagenum_t free_page_idx = header_page.free_page_idx;
	freepage_t free_page;

	pread64(database_fd, &free_page, PAGE_SIZE, free_page_idx * PAGE_SIZE);
	header_page.free_page_idx = free_page.next_free_idx;

	_flush_header();

	return free_page_idx;
}

void file_free_page(int fd, pagenum_t pagenum) {
	_switch_to_fd(fd);

	pagenum_t old_free_page_idx = header_page.free_page_idx;
	freepage_t new_free_page;

	new_free_page.next_free_idx = old_free_page_idx;
	pwrite64(database_fd, &new_free_page, PAGE_SIZE, pagenum * PAGE_SIZE);
	header_page.free_page_idx = pagenum;

	_flush_header();

	return;
}

void file_read_page(int fd, pagenum_t pagenum, page_t* dest) {
	_switch_to_fd(fd);
	pread64(database_fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE);
}

void file_write_page(int fd, pagenum_t pagenum, const page_t* src) {
	_switch_to_fd(fd);
	pwrite64(database_fd, src, PAGE_SIZE, pagenum * PAGE_SIZE);
}

void file_close_database_file() {
	for (
		int index = 0;
		index < database_instance_count;
		index++
	) {
		close(database_instances[index].file_descriptor);
		delete[] database_instances[index].file_path;
	}

	database_instance_count = 0;
	database_fd = 0;
}
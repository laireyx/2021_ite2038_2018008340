
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "errors.h"

/// @brief current database instance number
int database_instance_count = 0;
/// @brief all database instances
DatabaseInstance database_instances[MAX_DATABASE_INSTANCE];

/// @brief currently opened database file descriptor
int database_fd = 0;

/// @brief currently opened database header page
headerpage_t header_page;

namespace file_helper {
	void switch_to_fd(int fd) {
		assert(fd != 0);
		if(database_fd == fd) return;

		database_fd = fd;
		CHECK(pread64(database_fd, &header_page, PAGE_SIZE, 0));
	}

	void extend_capacity(pagenum_t newsize = 0) {
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

				CHECK(pwrite64(database_fd, &free_page, PAGE_SIZE, free_page_idx * PAGE_SIZE));
			}

			header_page.free_page_idx = header_page.page_num;
			header_page.page_num = newsize;
		}
	}

	void flush_header() {
		assert(database_fd != 0);
		CHECK(pwrite64(database_fd, &header_page, PAGE_SIZE, 0));
	}
};

int file_open_database_file(const char* path) {
	
	char* real_path = realpath(path, NULL);

	for (
		int index = 0;
		index < database_instance_count;
		index++
	) {
		if (
			strcmp(
				database_instances[index].file_path,
				real_path
			) == 0
		) {
			free(real_path);
			return index;
		}
	}

	if (database_instance_count >= MAX_DATABASE_INSTANCE) {
		free(real_path);
		return -1;
	}

	DatabaseInstance& new_instance = database_instances[database_instance_count++];
	new_instance.file_path = real_path;

	if ((database_fd = open(path, O_RDWR | O_SYNC)) < 1) {
		if(errno == ENOENT) {
			database_fd = open(path, O_RDWR | O_CREAT | O_SYNC, 0644);

			header_page.free_page_idx = 0;
			header_page.page_num = 1;

			file_helper::extend_capacity(2560);

			file_helper::flush_header();
		} else {
			return _perrno();
		}
	}
	else {
		CHECK(read(database_fd, &header_page, PAGE_SIZE));
	}

	return new_instance.file_descriptor = database_fd;
}

pagenum_t file_alloc_page(int fd) {
	file_helper::switch_to_fd(fd);
	file_helper::extend_capacity();

	pagenum_t free_page_idx = header_page.free_page_idx;
	freepage_t free_page;

	CHECK(pread64(database_fd, &free_page, PAGE_SIZE, free_page_idx * PAGE_SIZE));
	header_page.free_page_idx = free_page.next_free_idx;

	file_helper::flush_header();

	return free_page_idx;
}

void file_free_page(int fd, pagenum_t pagenum) {
	file_helper::switch_to_fd(fd);

	pagenum_t old_free_page_idx = header_page.free_page_idx;
	freepage_t new_free_page;

	new_free_page.next_free_idx = old_free_page_idx;
	CHECK(pwrite64(database_fd, &new_free_page, PAGE_SIZE, pagenum * PAGE_SIZE));
	header_page.free_page_idx = pagenum;

	file_helper::flush_header();

	return;
}

void file_read_page(int fd, pagenum_t pagenum, page_t* dest) {
	file_helper::switch_to_fd(fd);
	CHECK(pread64(database_fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE));
}

void file_write_page(int fd, pagenum_t pagenum, const page_t* src) {
	file_helper::switch_to_fd(fd);
	CHECK(pwrite64(database_fd, src, PAGE_SIZE, pagenum * PAGE_SIZE));
}

void file_close_database_file() {
	for (
		int index = 0;
		index < database_instance_count;
		index++
	) {
		close(database_instances[index].file_descriptor);
		free(database_instances[index].file_path);
	}

	database_instance_count = 0;
	database_fd = 0;
}
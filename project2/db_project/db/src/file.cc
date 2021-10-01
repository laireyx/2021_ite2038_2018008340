/**
 * @addtogroup DiskSpaceManager
 * @{
 */
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
bool switch_to_fd(int fd) {
    // file descriptor should be positive integer.
    assert(fd > 0);
    // If the given fd is equal to current fd, just return true.
    if (database_fd == fd) return true;

    // Switch database fd.
    database_fd = fd;
    // Switch header page content with new database fd.
    return error::ok(pread64(database_fd, &header_page, PAGE_SIZE, 0));
}

void extend_capacity(pagenum_t newsize = 0) {
    if (newsize > header_page.page_num || header_page.free_page_idx == 0) {
        if (newsize == 0) {
            newsize = header_page.page_num * 2;
        }

		// from page number to new size, create a new free page and write it.
        for (pagenum_t free_page_idx = header_page.page_num;
             free_page_idx < newsize; free_page_idx++) {
            freepage_t free_page;

			/// next free page index is next page index, unless it is the last page.
            if (free_page_idx < newsize - 1)
                free_page.next_free_idx = free_page_idx + 1;
            else
                free_page.next_free_idx = 0;

            error::ok(pwrite64(database_fd, &free_page, PAGE_SIZE,
                                  free_page_idx * PAGE_SIZE));
        	error::ok(fsync(database_fd));
        }

        header_page.free_page_idx = header_page.page_num;
        header_page.page_num = newsize;

		error::ok(pwrite64(database_fd, &header_page, PAGE_SIZE, 0));
        error::ok(fsync(database_fd));
    }
}

void flush_header() {
    assert(database_fd > 0);
    error::ok(pwrite64(database_fd, &header_page, PAGE_SIZE, 0));
    error::ok(fdatasync(database_fd));
}
};

int file_open_database_file(const char* path) {
	
	char* real_path = NULL;

	// If database instance is already full, then return error.
	if (database_instance_count >= MAX_DATABASE_INSTANCE) {
		return -1;
	}

	// Check if database file is already in database instance array.
	if((real_path = realpath(path, NULL)) > 0) {
		for (
			int instance_idx = 0;
			instance_idx < database_instance_count;
			instance_idx++
		) {
			if (
				strcmp(
					database_instances[instance_idx].file_path,
					real_path
				) == 0
			) {
				// If exists, then return it.
				free(real_path);
				return database_instances[instance_idx].file_descriptor;
			}
		}

		free(real_path);
	}

	// Reserve a new area in database instance array.
	DatabaseInstance& new_instance = database_instances[database_instance_count++];

	// Check if file exists.
	if ((database_fd = open(path, O_RDWR)) < 1) {
		if(errno == ENOENT) {
			// Create if not exists.
			error::ok(database_fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0644));

			// Initialize header page.
			header_page.free_page_idx = 0;
			header_page.page_num = 1;

			// Initialize free pages.
			file_helper::extend_capacity(2560);
		} else {
			return error::print();
		}
	}
	else {
		// Read header page.
		error::ok(read(database_fd, &header_page, PAGE_SIZE));
	}

	new_instance.file_path = realpath(path, NULL);

	return (new_instance.file_descriptor = database_fd);
}

pagenum_t file_alloc_page(int fd) {
    if (!file_helper::switch_to_fd(fd)) {
        return 0;
    }
	file_helper::extend_capacity();

	// Pop the first page from free page stack.
	pagenum_t free_page_idx = header_page.free_page_idx;
	freepage_t free_page;

	error::ok(pread64(database_fd, &free_page, PAGE_SIZE, free_page_idx * PAGE_SIZE));
	// Move the first free page index to the next page.
	header_page.free_page_idx = free_page.next_free_idx;

	file_helper::flush_header();

	return free_page_idx;
}

void file_free_page(int fd, pagenum_t pagenum) {
    if (!file_helper::switch_to_fd(fd)) {
        return;
    }

	// Current first free page index
	pagenum_t old_free_page_idx = header_page.free_page_idx;
	// Newly freed page
	freepage_t new_free_page;

	// Next free page index of newly freed page is current first free page index.
	// Its just pushing the pagenum into free page stack.
	new_free_page.next_free_idx = old_free_page_idx;
	error::ok(pwrite64(database_fd, &new_free_page, PAGE_SIZE, pagenum * PAGE_SIZE));
	error::ok(fdatasync(database_fd));

	// Set the first free page to freed page number.
	header_page.free_page_idx = pagenum;
	file_helper::flush_header();

	return;
}

void file_read_page(int fd, pagenum_t pagenum, page_t* dest) {
    if (!file_helper::switch_to_fd(fd)) {
        return;
    }
	error::ok(pread64(database_fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE));
}

void file_write_page(int fd, pagenum_t pagenum, const page_t* src) {
    if (!file_helper::switch_to_fd(fd)) {
        return;
    }
	error::ok(pwrite64(database_fd, src, PAGE_SIZE, pagenum * PAGE_SIZE));
	error::ok(fdatasync(database_fd));
}

void file_close_database_file() {
	for (
		int instance_idx = 0;
		instance_idx < database_instance_count;
		instance_idx++
	) {
		// Close file descriptor and free file path
		close(database_instances[instance_idx].file_descriptor);
		free(database_instances[instance_idx].file_path);

		// Reset for accidently re-opening database file.
		database_instances[instance_idx].file_descriptor = 0;
		database_instances[instance_idx].file_path = NULL;
	}

	// Clear database instance count and current database fd.
	database_instance_count = 0;
	database_fd = 0;
}
/** @}*/
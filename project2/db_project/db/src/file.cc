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

/// @brief current table instance number
int table_instance_count = 0;
/// @brief all table instances
TableInstance table_instances[MAX_TABLE_INSTANCE];

/// @brief currently opened table file descriptor
int table_fd = 0;

/// @brief currently opened table header page
headerpage_t header_page;

namespace file_helper {
bool switch_to_fd(int64_t table_id) {
    // file descriptor should be positive integer.
    assert(table_id > 0);
    // If the given fd is equal to current fd, just return true.
    if (table_fd == table_instances[table_id].file_descriptor) return true;

    // Switch table fd.
    table_fd = table_instances[table_id].file_descriptor;
    // Switch header page content with new table fd.
    return error::ok(pread64(table_fd, &header_page, PAGE_SIZE, 0) == PAGE_SIZE);
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

            error::ok(pwrite64(table_fd, &free_page, PAGE_SIZE,
                                  free_page_idx * PAGE_SIZE) == PAGE_SIZE);
        	error::ok(fsync(table_fd) == 0);
        }

        header_page.free_page_idx = header_page.page_num;
        header_page.page_num = newsize;

		error::ok(pwrite64(table_fd, &header_page, PAGE_SIZE, 0) == PAGE_SIZE);
        error::ok(fsync(table_fd) == 0);
    }
}

void flush_header() {
    assert(table_fd > 0);
    error::ok(pwrite64(table_fd, &header_page, PAGE_SIZE, 0) == PAGE_SIZE);
    error::ok(fsync(table_fd) == 0);
}
};

int init_db() {
	return 0;
}

int64_t file_open_table_file(const char* pathname) {
	
	char* real_path = NULL;

	// If table instance is already full, then return error.
	if (table_instance_count >= MAX_TABLE_INSTANCE) {
		return -1;
	}

	// Check if table file is already in table instance array.
	if((real_path = realpath(pathname, NULL)) > 0) {
		for (
			int instance_idx = 0;
			instance_idx < table_instance_count;
			instance_idx++
		) {
			if (
				strcmp(
					table_instances[instance_idx].file_path,
					real_path
				) == 0
			) {
				// If exists, then return it.
				free(real_path);
				return table_instances[instance_idx].file_descriptor;
			}
		}

		free(real_path);
	}

	// Reserve a new area in table instance array.
	TableInstance& new_instance = table_instances[table_instance_count];

	// Check if file exists.
	if ((table_fd = open(pathname, O_RDWR)) < 1) {
		if(errno == ENOENT) {
			// Create if not exists.
			error::ok((table_fd = open(pathname, O_RDWR | O_CREAT | O_EXCL, 0644)) > 0);

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
		error::ok(read(table_fd, &header_page, PAGE_SIZE) == PAGE_SIZE);
	}

	new_instance.file_path = realpath(pathname, NULL);
	new_instance.file_descriptor = table_fd;

	return table_instance_count++;
}

pagenum_t file_alloc_page(int64_t table_id) {
    if (!file_helper::switch_to_fd(table_id)) {
        return 0;
    }
	file_helper::extend_capacity();

	// Pop the first page from free page stack.
	pagenum_t free_page_idx = header_page.free_page_idx;
	freepage_t free_page;

	error::ok(pread64(table_fd, &free_page, PAGE_SIZE, free_page_idx * PAGE_SIZE) == PAGE_SIZE);
	// Move the first free page index to the next page.
	header_page.free_page_idx = free_page.next_free_idx;

	file_helper::flush_header();

	return free_page_idx;
}

void file_free_page(int64_t table_id, pagenum_t pagenum) {
    if (!file_helper::switch_to_fd(table_id)) {
        return;
    }

	// Current first free page index
	pagenum_t old_free_page_idx = header_page.free_page_idx;
	// Newly freed page
	freepage_t new_free_page;

	// Next free page index of newly freed page is current first free page index.
	// Its just pushing the pagenum into free page stack.
	new_free_page.next_free_idx = old_free_page_idx;
	error::ok(pwrite64(table_fd, &new_free_page, PAGE_SIZE, pagenum * PAGE_SIZE) == PAGE_SIZE);
	error::ok(fsync(table_fd) == 0);

	// Set the first free page to freed page number.
	header_page.free_page_idx = pagenum;
	file_helper::flush_header();

	return;
}

void file_read_page(int64_t table_id, pagenum_t pagenum, page_t* dest) {
    if (!file_helper::switch_to_fd(table_id)) {
        return;
    }
	error::ok(pread64(table_fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE) == PAGE_SIZE);
}

void file_write_page(int64_t table_id, pagenum_t pagenum, const page_t* src) {
    if (!file_helper::switch_to_fd(table_id)) {
        return;
    }
	error::ok(pwrite64(table_fd, src, PAGE_SIZE, pagenum * PAGE_SIZE) == PAGE_SIZE);
	error::ok(fsync(table_fd) == 0);
}

void file_close_table_files() {
	for (
		int instance_idx = 0;
		instance_idx < table_instance_count;
		instance_idx++
	) {
		// Close file descriptor and free file path
		close(table_instances[instance_idx].file_descriptor);
		free(table_instances[instance_idx].file_path);

		// Reset for accidently re-opening table file.
		table_instances[instance_idx].file_descriptor = 0;
		table_instances[instance_idx].file_path = NULL;
	}

	// Clear table instance count and current table fd.
	table_instance_count = 0;
	table_fd = 0;
}

int shutdown_db() {
	file_close_table_files();
	return 0;
}
/** @}*/
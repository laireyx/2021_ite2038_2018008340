

# db/src/file.cc



## Namespaces

| Name           |
| -------------- |
| **[file_helper](/Namespaces/file_helper)** <br>Filemanager helper.  |

## Functions

|                | Name           |
| -------------- | -------------- |
| tableid_t | **[file_open_table_file](/Modules/DiskSpaceManager#function-file_open_table_file)**(const char * path)<br>Open existing table file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Modules/DiskSpaceManager#function-file_alloc_page)**(int64_t table_id)<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Modules/DiskSpaceManager#function-file_free_page)**(int64_t table_id, pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Modules/DiskSpaceManager#function-file_read_page)**(int64_t table_id, pagenum_t pagenum, <a href="/Modules/DiskSpaceManager#typedef-page-t">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Modules/DiskSpaceManager#function-file_write_page)**(int64_t table_id, pagenum_t pagenum, const <a href="/Modules/DiskSpaceManager#typedef-page-t">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_table_files](/Modules/DiskSpaceManager#function-file_close_table_files)**()<br>Stop referencing the table files.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[table_instance_count](/Modules/DiskSpaceManager#variable-table_instance_count)** <br>current table instance number  |
| <a href="/Classes/TableInstance">TableInstance</a> | **[table_instances](/Modules/DiskSpaceManager#variable-table_instances)** <br>all table instances  |


## Functions Documentation

### function file_open_table_file

```cpp
tableid_t file_open_table_file(
    const char * path
)
```

Open existing table file or create one if not existed. 

**Parameters**: 

  * **path** Table file path. 


**Return**: ID of the opened table file. 

### function file_alloc_page

```cpp
pagenum_t file_alloc_page(
    int64_t table_id
)
```

Allocate an on-disk page from the free page list. 

**Parameters**: 

  * **table_id** table id obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-table-file">file&#95;open&#95;table&#95;file()</a></code>. 


**Return**: >0 <a href="/Classes/Page">Page</a> index number if allocation success. 0 Zero if allocation failed. 

### function file_free_page

```cpp
void file_free_page(
    int64_t table_id,
    pagenum_t pagenum
)
```

Free an on-disk page to the free page list. 

**Parameters**: 

  * **table_id** table id obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-table-file">file&#95;open&#95;table&#95;file()</a></code>. 
  * **pagenum** page index. 


### function file_read_page

```cpp
void file_read_page(
    int64_t table_id,
    pagenum_t pagenum,
    page_t * dest
)
```

Read an on-disk page into the in-memory page structure(dest) 

**Parameters**: 

  * **table_id** table id obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-table-file">file&#95;open&#95;table&#95;file()</a></code>. 
  * **pagenum** page index. 
  * **dest** the pointer of the page data. 


### function file_write_page

```cpp
void file_write_page(
    int64_t table_id,
    pagenum_t pagenum,
    const page_t * src
)
```

Write an in-memory page(src) to the on-disk page. 

**Parameters**: 

  * **table_id** table id obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-table-file">file&#95;open&#95;table&#95;file()</a></code>. 
  * **pagenum** page index. 
  * **src** the pointer of the page data. 


### function file_close_table_files

```cpp
void file_close_table_files()
```

Stop referencing the table files. 


## Attributes Documentation

### variable table_instance_count

```cpp
int table_instance_count = 0;
```

current table instance number 

### variable table_instances

```cpp
TableInstance table_instances;
```

all table instances 


## Source code

```cpp

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "file.h"
#include "errors.h"

int table_instance_count = 0;
TableInstance table_instances[MAX_TABLE_INSTANCE];

namespace file_helper {
TableInstance& get_table(tableid_t table_id) {
    error::ok(table_id < table_instance_count);
    return table_instances[table_id];
}

void extend_capacity(tableid_t table_id, pagenum_t newsize = 0) {
    auto& instance = get_table(table_id);

    int table_fd = instance.file_descriptor;
    headerpage_t& header_page = instance.header_page;

    if (newsize > header_page.page_num || header_page.free_page_idx == 0) {
        if (newsize == 0) {
            newsize = header_page.page_num * 2;
        }

        // from page number to new size, create a new free page and write it.
        for (pagenum_t free_page_idx = header_page.page_num;
             free_page_idx < newsize; free_page_idx++) {
            freepage_t free_page;

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

void flush_header(tableid_t table_id) {
    auto& instance = get_table(table_id);

    int table_fd = instance.file_descriptor;
    headerpage_t& header_page = instance.header_page;

    error::ok(pwrite64(table_fd, &header_page, PAGE_SIZE, 0) == PAGE_SIZE);
    error::ok(fsync(table_fd) == 0);
}
};

tableid_t file_open_table_file(const char* pathname) {
    char* real_path = NULL;

    // If table instance is already full, then return error.
    if (table_instance_count >= MAX_TABLE_INSTANCE) {
        return -1;
    }

    // Check if table file is already in table instance array.
    if ((real_path = realpath(pathname, NULL)) > 0) {
        for (int instance_idx = 0; instance_idx < table_instance_count;
             instance_idx++) {
            if (strcmp(table_instances[instance_idx].file_path, real_path) ==
                0) {
                // If exists, then return it.
                free(real_path);
                return instance_idx;
            }
        }

        free(real_path);
    }

    // Reserve a new area in table instance array.
    auto& new_instance = file_helper::get_table(table_instance_count);

    int& table_fd = new_instance.file_descriptor;
    headerpage_t& header_page = new_instance.header_page;

    // Check if file exists.
    if ((table_fd = open(pathname, O_RDWR)) < 1) {
        if (errno == ENOENT) {
            // Create if not exists.
            error::ok((table_fd = open(pathname, O_RDWR | O_CREAT | O_EXCL,
                                       0644)) > 0);

            // Initialize header page.
            header_page.root_page_idx = 0;
            header_page.free_page_idx = 0;
            header_page.page_num = 1;

            // Initialize free pages.
            file_helper::extend_capacity(table_instance_count, 2560);
        } else {
            return error::print();
        }
    } else {
        // Read header page.
        error::ok(read(table_fd, &header_page, PAGE_SIZE) == PAGE_SIZE);
    }

    new_instance.file_path = realpath(pathname, NULL);

    return table_instance_count++;
}

pagenum_t file_alloc_page(int64_t table_id) {
    auto& instance = file_helper::get_table(table_id);

    int table_fd = instance.file_descriptor;
    headerpage_t& header_page = instance.header_page;

    file_helper::extend_capacity(table_id);

    // Pop the first page from free page stack.
    pagenum_t free_page_idx = header_page.free_page_idx;
    freepage_t free_page;

    error::ok(pread64(table_fd, &free_page, PAGE_SIZE,
                      free_page_idx * PAGE_SIZE) == PAGE_SIZE);
    // Move the first free page index to the next page.
    header_page.free_page_idx = free_page.next_free_idx;

    file_helper::flush_header(table_fd);

    return free_page_idx;
}

void file_free_page(int64_t table_id, pagenum_t pagenum) {
    auto& instance = file_helper::get_table(table_id);

    int table_fd = instance.file_descriptor;
    headerpage_t& header_page = instance.header_page;

    // Current first free page index
    pagenum_t old_free_page_idx = header_page.free_page_idx;
    // Newly freed page
    freepage_t new_free_page;

    // Next free page index of newly freed page is current first free page
    // index. Its just pushing the pagenum into free page stack.
    new_free_page.next_free_idx = old_free_page_idx;
    error::ok(pwrite64(table_fd, &new_free_page, PAGE_SIZE,
                       pagenum * PAGE_SIZE) == PAGE_SIZE);
    error::ok(fsync(table_fd) == 0);

    // Set the first free page to freed page number.
    header_page.free_page_idx = pagenum;
    file_helper::flush_header(table_id);

    return;
}

void file_read_page(int64_t table_id, pagenum_t pagenum, page_t* dest) {
    auto& instance = file_helper::get_table(table_id);

    int table_fd = instance.file_descriptor;
    error::ok(pread64(table_fd, dest, PAGE_SIZE, pagenum * PAGE_SIZE) ==
              PAGE_SIZE);
}

void file_write_page(int64_t table_id, pagenum_t pagenum, const page_t* src) {
    auto& instance = file_helper::get_table(table_id);

    int table_fd = instance.file_descriptor;
    error::ok(pwrite64(table_fd, src, PAGE_SIZE, pagenum * PAGE_SIZE) ==
              PAGE_SIZE);
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

    // Clear table instance count.
    table_instance_count = 0;
}
```


-------------------------------

Updated on 2021-10-16 at 20:56:45 +0900

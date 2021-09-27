

# filemanager/file.cc



## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[_switch_to_fd](/Files/filemanager/file.cc#function-_switch_to_fd)**(int fd)<br>Switch current database into given database.  |
| void | **[_extend_capacity](/Files/filemanager/file.cc#function-_extend_capacity)**(pagenum_t newsize)<br>Automatically check and size-up a page file.  |
| void | **[_flush_header](/Files/filemanager/file.cc#function-_flush_header)**()<br>Flush a header page as "pagenum 0".  |
| int64_t | **[file_open_database_file](/Files/filemanager/file.cc#function-file_open_database_file)**(const char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Files/filemanager/file.cc#function-file_alloc_page)**(int fd)<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Files/filemanager/file.cc#function-file_free_page)**(int fd, pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Files/filemanager/file.cc#function-file_read_page)**(int fd, pagenum_t pagenum, <a href="/Classes/Page">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Files/filemanager/file.cc#function-file_write_page)**(int fd, pagenum_t pagenum, const <a href="/Classes/Page">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Files/filemanager/file.cc#function-file_close_database_file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[database_instance_count](/Files/filemanager/file.cc#variable-database_instance_count)** <br>current database instance number  |
| <a href="/Classes/DatabaseInstance">DatabaseInstance</a> | **[database_instances](/Files/filemanager/file.cc#variable-database_instances)** <br>all database instances  |
| int | **[database_fd](/Files/filemanager/file.cc#variable-database_fd)** <br>currently opened database file descriptor  |
| <a href="/Classes/HeaderPage">headerpage_t</a> | **[header_page](/Files/filemanager/file.cc#variable-header_page)** <br>currently opened database header page  |


## Functions Documentation

### function _switch_to_fd

```cpp
void _switch_to_fd(
    int fd
)
```

Switch current database into given database. 

**Parameters**: 

  * **fd** Database file descriptor gained with file_open_database_file. 


If current database_fd is equal to given fd, then do nothing. If not, change database_fd to given fd and re-read header_page from it.


### function _extend_capacity

```cpp
void _extend_capacity(
    pagenum_t newsize
)
```

Automatically check and size-up a page file. 

**Parameters**: 

  * **newsize** extended size. default is 0, which means doubling the reserved page count if there are no free page. 


If newsize > page_num, reserve pages so that total page num is equivalent to newsize. If newsize = 0 and header page's free_page_idx is 0, double the reserved page count.


### function _flush_header

```cpp
void _flush_header()
```

Flush a header page as "pagenum 0". 

Write header page into offset 0 of the current database file descriptor. 


### function file_open_database_file

```cpp
int64_t file_open_database_file(
    const char * path
)
```

Open existing database file or create one if not existed. 

**Parameters**: 

  * **path** Database file path. 


**Return**: ID of the opened database file. 

### function file_alloc_page

```cpp
pagenum_t file_alloc_page(
    int fd
)
```

Allocate an on-disk page from the free page list. 

**Parameters**: 

  * **fd** Database file descriptor gained with file_open_database_file. 


**Return**: Allocated page index. 

### function file_free_page

```cpp
void file_free_page(
    int fd,
    pagenum_t pagenum
)
```

Free an on-disk page to the free page list. 

**Parameters**: 

  * **fd** Database file descriptor gained with file_open_database_file. 
  * **pagenum** page index. 


### function file_read_page

```cpp
void file_read_page(
    int fd,
    pagenum_t pagenum,
    page_t * dest
)
```

Read an on-disk page into the in-memory page structure(dest) 

**Parameters**: 

  * **fd** Database file descriptor gained with file_open_database_file. 
  * **pagenum** page index. 
  * **dest** the pointer of the page data. 


### function file_write_page

```cpp
void file_write_page(
    int fd,
    pagenum_t pagenum,
    const page_t * src
)
```

Write an in-memory page(src) to the on-disk page. 

**Parameters**: 

  * **fd** Database file descriptor gained with file_open_database_file. 
  * **pagenum** page index. 
  * **src** the pointer of the page data. 


### function file_close_database_file

```cpp
void file_close_database_file()
```

Stop referencing the database file. 


## Attributes Documentation

### variable database_instance_count

```cpp
int database_instance_count = 0;
```

current database instance number 

### variable database_instances

```cpp
DatabaseInstance database_instances;
```

all database instances 

### variable database_fd

```cpp
int database_fd = 0;
```

currently opened database file descriptor 

### variable header_page

```cpp
headerpage_t header_page;
```

currently opened database header page 


## Source code

```cpp

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <cassert>
#include "file.h"

int database_instance_count = 0;
DatabaseInstance database_instances[MAX_DATABASE_INSTANCE];

int database_fd = 0;

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

int64_t file_open_database_file(const char* path) {
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
    new_instance.file_path = reinterpret_cast<char*>(malloc(sizeof(char) * (strlen(path) + 1)));
    strncpy(new_instance.file_path, path, strlen(path) + 1);

    if ((database_fd = open(path, O_RDWR | O_SYNC, 0644)) < 1) {
        database_fd = open(path, O_RDWR | O_CREAT | O_SYNC, 0644);

        header_page.free_page_idx = 0;
        header_page.page_num = 1;

        _extend_capacity(2560);

        _flush_header();
    }
    else {
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
    }

    database_instance_count = 0;
    database_fd = 0;
}
```


-------------------------------

Updated on 2021-09-27 at 20:57:40 +0900



# db/include/file.h



## Namespaces

| Name           |
| -------------- |
| **[file_helper](/Namespaces/file_helper)** <br>Filemanager helper.  |

## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[file_open_database_file](/Files/db/include/file.h#function-file_open_database_file)**(const char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Files/db/include/file.h#function-file_alloc_page)**(int fd)<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Files/db/include/file.h#function-file_free_page)**(int fd, pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Files/db/include/file.h#function-file_read_page)**(int fd, pagenum_t pagenum, <a href="/Classes/Page">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Files/db/include/file.h#function-file_write_page)**(int fd, pagenum_t pagenum, const <a href="/Classes/Page">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Files/db/include/file.h#function-file_close_database_file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[INITIAL_DB_FILE_SIZE](/Files/db/include/file.h#variable-initial_db_file_size)** <br>Initial size(in bytes) of newly created database file.  |
| constexpr int | **[MAX_DATABASE_INSTANCE](/Files/db/include/file.h#variable-max_database_instance)** <br>Maximum number of database instances count.  |
| constexpr int | **[INITIAL_DATABASE_CAPS](/Files/db/include/file.h#variable-initial_database_caps)** <br>Initial number of page count in newly created database file.  |


## Functions Documentation

### function file_open_database_file

```cpp
int file_open_database_file(
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

  * **fd** Database file descriptor obtained with <code>file&#95;open&#95;database&#95;file()</code>. 


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

  * **fd** Database file descriptor obtained with <code>file&#95;open&#95;database&#95;file()</code>. 
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

  * **fd** Database file descriptor obtained with <code>file&#95;open&#95;database&#95;file()</code>. 
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

  * **fd** Database file descriptor obtained with <code>file&#95;open&#95;database&#95;file()</code>. 
  * **pagenum** page index. 
  * **src** the pointer of the page data. 


### function file_close_database_file

```cpp
void file_close_database_file()
```

Stop referencing the database file. 


## Attributes Documentation

### variable INITIAL_DB_FILE_SIZE

```cpp
constexpr int INITIAL_DB_FILE_SIZE = 10 * 1024 * 1024;
```

Initial size(in bytes) of newly created database file. 

It means 10MiB. 


### variable MAX_DATABASE_INSTANCE

```cpp
constexpr int MAX_DATABASE_INSTANCE = 1024;
```

Maximum number of database instances count. 

### variable INITIAL_DATABASE_CAPS

```cpp
constexpr int INITIAL_DATABASE_CAPS =
    INITIAL_DB_FILE_SIZE / MAX_DATABASE_INSTANCE;
```

Initial number of page count in newly created database file. 

Its value is 2560. 



## Source code

```cpp
#pragma once

#include "types.h"

constexpr int INITIAL_DB_FILE_SIZE = 10 * 1024 * 1024;

constexpr int MAX_DATABASE_INSTANCE = 1024;

constexpr int INITIAL_DATABASE_CAPS =
    INITIAL_DB_FILE_SIZE / MAX_DATABASE_INSTANCE;

namespace file_helper {
void switch_to_fd(int fd);

void extend_capacity(pagenum_t newsize);

void flush_header();
};  // namespace file_helper

int file_open_database_file(const char* path);

pagenum_t file_alloc_page(int fd);

void file_free_page(int fd, pagenum_t pagenum);

void file_read_page(int fd, pagenum_t pagenum, page_t* dest);

void file_write_page(int fd, pagenum_t pagenum, const page_t* src);

void file_close_database_file();
```


-------------------------------

Updated on 2021-09-29 at 22:55:07 +0900

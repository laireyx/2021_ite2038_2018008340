

# filemanager/file.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[DatabaseInstance](/Classes/DatabaseInstance)** <br>Database file instance.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef struct <a href="/Classes/DatabaseInstance">DatabaseInstance</a> | **[DatabaseInstance](/Files/filemanager/file.h#typedef-databaseinstance)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[_switch_to_fd](/Files/filemanager/file.h#function-_switch_to_fd)**(int fd)<br>Switch current database into given database.  |
| void | **[_extend_capacity](/Files/filemanager/file.h#function-_extend_capacity)**(pagenum_t newsize)<br>Automatically check and size-up a page file.  |
| void | **[_flush_header](/Files/filemanager/file.h#function-_flush_header)**()<br>Flush a header page as "pagenum 0".  |
| int | **[file_open_database_file](/Files/filemanager/file.h#function-file_open_database_file)**(const char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Files/filemanager/file.h#function-file_alloc_page)**(int fd)<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Files/filemanager/file.h#function-file_free_page)**(int fd, pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Files/filemanager/file.h#function-file_read_page)**(int fd, pagenum_t pagenum, <a href="/Classes/Page">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Files/filemanager/file.h#function-file_write_page)**(int fd, pagenum_t pagenum, const <a href="/Classes/Page">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Files/filemanager/file.h#function-file_close_database_file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[MAX_DATABASE_INSTANCE](/Files/filemanager/file.h#variable-max-database-instance)** <br>Maximum number of database instances count.  |

## Types Documentation

### typedef DatabaseInstance

```cpp
typedef struct DatabaseInstance DatabaseInstance;
```



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

### variable MAX_DATABASE_INSTANCE

```cpp
constexpr int MAX_DATABASE_INSTANCE = 1024;
```

Maximum number of database instances count. 


## Source code

```cpp
#pragma once

#include "types.h"

constexpr int MAX_DATABASE_INSTANCE = 1024;

typedef struct DatabaseInstance {
    char* file_path;
    int file_descriptor;
} DatabaseInstance;

void _switch_to_fd(int fd);

void _extend_capacity(pagenum_t newsize);

void _flush_header();

int file_open_database_file(const char* path);

pagenum_t file_alloc_page(int fd);

void file_free_page(int fd, pagenum_t pagenum);

void file_read_page(int fd, pagenum_t pagenum, page_t* dest);

void file_write_page(int fd, pagenum_t pagenum, const page_t* src);

void file_close_database_file();
```


-------------------------------

Updated on 2021-09-28 at 10:05:21 +0900

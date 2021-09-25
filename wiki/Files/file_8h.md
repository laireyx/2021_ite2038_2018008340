---
title: filemanager/file.h

---

# filemanager/file.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[DatabaseInstance](/Classes/structDatabaseInstance)** <br>Database file instance.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef struct [DatabaseInstance](/Classes/structDatabaseInstance) | **[DatabaseInstance](/Files/file_8h#typedef-databaseinstance)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| int64_t | **[file_open_database_file](/Files/file_8h#function-file-open-database-file)**(char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Files/file_8h#function-file-alloc-page)**()<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Files/file_8h#function-file-free-page)**(pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Files/file_8h#function-file-read-page)**(pagenum_t pagenum, [page_t](/Classes/structPage) * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Files/file_8h#function-file-write-page)**(pagenum_t pagenum, const [page_t](/Classes/structPage) * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Files/file_8h#function-file-close-database-file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[MAX_DATABASE_INSTANCE](/Files/file_8h#variable-max-database-instance)** <br>Maximum number of database instances count.  |

## Types Documentation

### typedef DatabaseInstance

```cpp
typedef struct DatabaseInstance DatabaseInstance;
```



## Functions Documentation

### function file_open_database_file

```cpp
int64_t file_open_database_file(
    char * path
)
```

Open existing database file or create one if not existed. 

**Parameters**: 

  * **path** Database file. 


**Return**: ID of the opened database file. 

### function file_alloc_page

```cpp
pagenum_t file_alloc_page()
```

Allocate an on-disk page from the free page list. 

**Return**: The very free page index. 

### function file_free_page

```cpp
void file_free_page(
    pagenum_t pagenum
)
```

Free an on-disk page to the free page list. 

**Parameters**: 

  * **pagenum** page index. 


### function file_read_page

```cpp
void file_read_page(
    pagenum_t pagenum,
    page_t * dest
)
```

Read an on-disk page into the in-memory page structure(dest) 

**Parameters**: 

  * **pagenum** page index. 
  * **dest** the pointer of the page data. 


### function file_write_page

```cpp
void file_write_page(
    pagenum_t pagenum,
    const page_t * src
)
```

Write an in-memory page(src) to the on-disk page. 

**Parameters**: 

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
    FILE* file_pointer;
} DatabaseInstance;

int64_t file_open_database_file(char* path);

pagenum_t file_alloc_page();

void file_free_page(pagenum_t pagenum);

void file_read_page(pagenum_t pagenum, page_t* dest);

void file_write_page(pagenum_t pagenum, const page_t* src);

void file_close_database_file();
```


-------------------------------

Updated on 2021-09-26 at 01:11:28 +0900

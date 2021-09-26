---
title: filemanager/file.cc

---

# filemanager/file.cc



## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[_extend_capacity](/Files/file_8cc#function--extend-capacity)**(pagenum_t newsize =0)<br>Automatically check and size-up a page file.  |
| void | **[_seek_page](/Files/file_8cc#function--seek-page)**(pagenum_t pagenum)<br>Seek page file pointer at offset matching with given page index.  |
| void | **[_flush_header](/Files/file_8cc#function--flush-header)**()<br>Flush a header page as "pagenum 0".  |
| int64_t | **[file_open_database_file](/Files/file_8cc#function-file-open-database-file)**(const char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Files/file_8cc#function-file-alloc-page)**()<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Files/file_8cc#function-file-free-page)**(pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Files/file_8cc#function-file-read-page)**(pagenum_t pagenum, [page_t](/Classes/structPage) * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Files/file_8cc#function-file-write-page)**(pagenum_t pagenum, const [page_t](/Classes/structPage) * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Files/file_8cc#function-file-close-database-file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[database_instance_count](/Files/file_8cc#variable-database-instance-count)** <br>current database instance number  |
| [DatabaseInstance](/Classes/structDatabaseInstance) | **[database_instances](/Files/file_8cc#variable-database-instances)** <br>all database instances  |
| FILE * | **[database_file](/Files/file_8cc#variable-database-file)** <br>currently opened database file pointer  |
| [headerpage_t](/Classes/structHeaderPage) | **[header_page](/Files/file_8cc#variable-header-page)** <br>currently opened database header page  |


## Functions Documentation

### function _extend_capacity

```cpp
void _extend_capacity(
    pagenum_t newsize =0
)
```

Automatically check and size-up a page file. 

**Parameters**: 

  * **newsize** extended size. default is 0, which means doubleing the reserved page count if there are no free page. 


Extend capacity if newsize if specified. Or if there are no space for the next free page, double the reserved page count.


### function _seek_page

```cpp
void _seek_page(
    pagenum_t pagenum
)
```

Seek page file pointer at offset matching with given page index. 

**Parameters**: 

  * **pagenum** page index. 


### function _flush_header

```cpp
void _flush_header()
```

Flush a header page as "pagenum 0". 

### function file_open_database_file

```cpp
int64_t file_open_database_file(
    const char * path
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

### variable database_file

```cpp
FILE * database_file = nullptr;
```

currently opened database file pointer 

### variable header_page

```cpp
headerpage_t header_page;
```

currently opened database header page 


## Source code

```cpp

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "file.h"

int database_instance_count = 0;
DatabaseInstance database_instances[MAX_DATABASE_INSTANCE + 1];

FILE* database_file = nullptr;
headerpage_t header_page;

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

void _seek_page(pagenum_t pagenum) {
    assert(database_file != nullptr);
    fseek(database_file, pagenum * PAGE_SIZE, SEEK_SET);
}

void _flush_header() {
    assert(database_file != nullptr);
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
    assert(database_file != nullptr);
    _extend_capacity();

    pagenum_t free_page_idx = header_page.free_page_idx;
    freepage_t free_page;

    file_read_page(free_page_idx, reinterpret_cast<page_t*>(&free_page));
    header_page.free_page_idx = free_page.next_free_idx;

    _flush_header();

    return free_page_idx;
}

void file_free_page(pagenum_t pagenum) {
    assert(database_file != nullptr);
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

    database_instance_count = 0;
    database_file = nullptr;
}
```


-------------------------------

Updated on 2021-09-26 at 23:53:21 +0900

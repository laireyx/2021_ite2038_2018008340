

# filemanager/file.cc



## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[_seek_page](/Files/filemanager/file.cc#function-_seek_page)**(pagenum_t pagenum)<br>Seek page file pointer at offset matching with given page index.  |
| void | **[_extend_capacity](/Files/filemanager/file.cc#function-_extend_capacity)**(pagenum_t newsize)<br>Automatically check and size-up a page file.  |
| void | **[_flush_header](/Files/filemanager/file.cc#function-_flush_header)**()<br>Flush a header page as "pagenum 0".  |
| int64_t | **[file_open_database_file](/Files/filemanager/file.cc#function-file_open_database_file)**(const char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Files/filemanager/file.cc#function-file_alloc_page)**()<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Files/filemanager/file.cc#function-file_free_page)**(pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Files/filemanager/file.cc#function-file_read_page)**(pagenum_t pagenum, <a href="/Classes/Page">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Files/filemanager/file.cc#function-file_write_page)**(pagenum_t pagenum, const <a href="/Classes/Page">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Files/filemanager/file.cc#function-file_close_database_file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| int | **[database_instance_count](/Files/filemanager/file.cc#variable-database_instance_count)** <br>current database instance number  |
| <a href="/Classes/DatabaseInstance">DatabaseInstance</a> | **[database_instances](/Files/filemanager/file.cc#variable-database_instances)** <br>all database instances  |
| FILE * | **[database_file](/Files/filemanager/file.cc#variable-database_file)** <br>currently opened database file pointer  |
| <a href="/Classes/HeaderPage">headerpage_t</a> | **[header_page](/Files/filemanager/file.cc#variable-header_page)** <br>currently opened database header page  |


## Functions Documentation

### function _seek_page

```cpp
void _seek_page(
    pagenum_t pagenum
)
```

Seek page file pointer at offset matching with given page index. 

**Parameters**: 

  * **pagenum** page index. 


### function _extend_capacity

```cpp
void _extend_capacity(
    pagenum_t newsize
)
```

Automatically check and size-up a page file. 

**Parameters**: 

  * **newsize** extended size. default is 0, which means doubleing the reserved page count if there are no free page. 


Extend capacity if newsize if specified. Or if there are no space for the next free page, double the reserved page count.


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

  * **path** Database file path. 


**Return**: ID of the opened database file. 

### function file_alloc_page

```cpp
pagenum_t file_alloc_page()
```

Allocate an on-disk page from the free page list. 

**Return**: Allocated page index. 

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

void _seek_page(pagenum_t pagenum) {
    assert(database_file != nullptr);
    fseek(database_file, pagenum * PAGE_SIZE, SEEK_SET);
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
            pagenum_t free_page_index = header_page.page_num;
            free_page_index < newsize;
            free_page_index++
        ) {

            freepage_t free_page;

            if (free_page_index < newsize - 1)
                free_page.next_free_idx = free_page_index + 1;
            else
                free_page.next_free_idx = 0;

            _seek_page(free_page_index);
            fwrite(&free_page, PAGE_SIZE, 1, database_file);
            fflush(database_file);
        }

        header_page.free_page_idx = header_page.page_num;
        header_page.page_num = newsize;
    }
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

    _seek_page(free_page_idx);
    fread(&free_page, PAGE_SIZE, 1, database_file);
    header_page.free_page_idx = free_page.next_free_idx;

    _flush_header();

    return free_page_idx;
}

void file_free_page(pagenum_t pagenum) {
    assert(database_file != nullptr);
    pagenum_t old_free_page_idx = header_page.free_page_idx;
    freepage_t new_free_page;

    new_free_page.next_free_idx = old_free_page_idx;
    _seek_page(pagenum);
    fwrite(&new_free_page, PAGE_SIZE, 1, database_file);
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

Updated on 2021-09-27 at 14:58:06 +0900
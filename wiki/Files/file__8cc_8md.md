---
title: file_8cc.md

---

# file_8cc.md






## Source code

```markdown
---
title: file.cc

---

# file.cc



## Functions

|                | Name           |
| -------------- | -------------- |
| void | **[_initialize](Files/file_8cc.md#function--initialize)**()<br>Initialized file manager.  |
| void | **[_page_growth](Files/file_8cc.md#function--page-growth)**()<br>Automatically check and size-up a page file.  |
| void | **[_seek_page](Files/file_8cc.md#function--seek-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum)<br>Seek page file pointer at offset matching with given page index.  |
| void | **[_flush_header](Files/file_8cc.md#function--flush-header)**()<br>Flush a header page as "pagenum 0".  |
| [pagenum_t](Modules/group__Type.md#typedef-pagenum-t) | **[file_alloc_page](Modules/group__File.md#function-file-alloc-page)**()<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](Modules/group__File.md#function-file-free-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](Modules/group__File.md#function-file-read-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum, [page_t](Modules/group__Type.md#typedef-page-t) * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](Modules/group__File.md#function-file-write-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum, const [page_t](Modules/group__Type.md#typedef-page-t) * src)<br>Write an in-memory page(src) to the on-disk page.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[initialized](Files/file_8cc.md#variable-initialized)**  |
| FILE * | **[page_file](Files/file_8cc.md#variable-page-file)**  |
| [headerpage_t](Modules/group__Type.md#typedef-headerpage-t) | **[header_page](Files/file_8cc.md#variable-header-page)**  |


## Functions Documentation

### function _initialize

```cpp
void _initialize()
```

Initialized file manager. 

Open a page file and read header page, or create and initialize if not exists. 


### function _page_growth

```cpp
void _page_growth()
```

Automatically check and size-up a page file. 

If there are no space for the next free page, double the reserved page count. 


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



## Attributes Documentation

### variable initialized

```cpp
bool initialized = false;
```


### variable page_file

```cpp
FILE * page_file;
```


### variable header_page

```cpp
headerpage_t header_page;
```



## Source code

```cpp

#include <cstdio>
#include "file.h"
#include "preferences.h"

bool initialized = false;
FILE* page_file;
headerpage_t header_page;

void _initialize() {
    if ((page_file = fopen(FILE_NAME, "r+b")) == NULL) {
        page_file = fopen(FILE_NAME, "w+b");

        header_page.free_page_idx = 0;
        header_page.page_num = 1;

        fwrite(&header_page, PAGE_SIZE, 1, page_file);
        fflush(page_file);
    }
    else {
        fread(&header_page, PAGE_SIZE, 1, page_file);
    }
    initialized = true;
}

void _page_growth() {
    if (header_page.free_page_idx == 0) {
        for (
            pagenum_t free_page_index = header_page.page_num;
            free_page_index < header_page.page_num * 2;
            free_page_index++
            ) {

            freepage_t free_page;

            if (free_page_index < header_page.page_num * 2 - 1)
                free_page.next_free_idx = free_page_index + 1;
            else
                free_page.next_free_idx = 0;

            file_write_page(free_page_index, reinterpret_cast<page_t*>(&free_page));
        }

        header_page.free_page_idx = header_page.page_num;
        header_page.page_num *= 2;
    }
}

void _seek_page(pagenum_t pagenum) {
    fseek(page_file, pagenum * PAGE_SIZE, SEEK_SET);
}

void _flush_header() {
    fseek(page_file, 0, SEEK_SET);
    fwrite(&header_page, PAGE_SIZE, 1, page_file);
    fflush(page_file);
}

pagenum_t file_alloc_page() {
    if (!initialized) _initialize();

    _page_growth();

    pagenum_t free_page_idx = header_page.free_page_idx;
    freepage_t free_page;

    file_read_page(free_page_idx, reinterpret_cast<page_t*>(&free_page));
    header_page.free_page_idx = free_page.next_free_idx;

    _flush_header();

    return free_page_idx;
}

void file_free_page(pagenum_t pagenum) {
    pagenum_t old_free_page_idx = header_page.free_page_idx;
    freepage_t new_free_page;

    new_free_page.next_free_idx = old_free_page_idx;
    file_write_page(pagenum, reinterpret_cast<page_t*>(&new_free_page));
    header_page.free_page_idx = pagenum;

    _flush_header();

    return;
}

void file_read_page(pagenum_t pagenum, page_t* dest) {
    if (!initialized) _initialize();
    _seek_page(pagenum);
    fread(dest, PAGE_SIZE, 1, page_file);
}

void file_write_page(pagenum_t pagenum, const page_t* src) {
    if (!initialized) _initialize();
    _seek_page(pagenum);
    fwrite(src, PAGE_SIZE, 1, page_file);
    fflush(page_file);
}
```


-------------------------------

Updated on 2021-09-25 at 17:41:34 +0900
```


-------------------------------

Updated on 2021-09-25 at 17:48:29 +0900

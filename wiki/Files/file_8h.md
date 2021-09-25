---
title: filemanager/file.h

---

# filemanager/file.h



## Functions

|                | Name           |
| -------------- | -------------- |
| pagenum_t | **[file_alloc_page](Files/file_8h.md#function-file-alloc-page)**()<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](Files/file_8h.md#function-file-free-page)**(pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](Files/file_8h.md#function-file-read-page)**(pagenum_t pagenum, [page_t](Classes/structPage.md) * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](Files/file_8h.md#function-file-write-page)**(pagenum_t pagenum, const [page_t](Classes/structPage.md) * src)<br>Write an in-memory page(src) to the on-disk page.  |


## Functions Documentation

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




## Source code

```cpp
#pragma once

#include "types.h"

pagenum_t file_alloc_page();

void file_free_page(pagenum_t pagenum);

void file_read_page(pagenum_t pagenum, page_t* dest);

void file_write_page(pagenum_t pagenum, const page_t* src);
```


-------------------------------

Updated on 2021-09-25 at 19:40:44 +0900

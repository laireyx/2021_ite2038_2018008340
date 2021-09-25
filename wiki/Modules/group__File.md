---
title: File Manager
summary: These are the file management functions. 

---

# File Manager

These are the file management functions. 

## Functions

|                | Name           |
| -------------- | -------------- |
| [pagenum_t](Modules/group__Type.md#typedef-pagenum-t) | **[file_alloc_page](Modules/group__File.md#function-file-alloc-page)**()<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](Modules/group__File.md#function-file-free-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](Modules/group__File.md#function-file-read-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum, [page_t](Modules/group__Type.md#typedef-page-t) * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](Modules/group__File.md#function-file-write-page)**([pagenum_t](Modules/group__Type.md#typedef-pagenum-t) pagenum, const [page_t](Modules/group__Type.md#typedef-page-t) * src)<br>Write an in-memory page(src) to the on-disk page.  |


## Functions Documentation

### function file_alloc_page

```
pagenum_t file_alloc_page()
```

Allocate an on-disk page from the free page list. 

**Return**: The very free page index. 

### function file_free_page

```
void file_free_page(
    pagenum_t pagenum
)
```

Free an on-disk page to the free page list. 

**Parameters**: 

  * **pagenum** page index. 


### function file_read_page

```
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

```
void file_write_page(
    pagenum_t pagenum,
    const page_t * src
)
```

Write an in-memory page(src) to the on-disk page. 

**Parameters**: 

  * **pagenum** page index. 
  * **src** the pointer of the page data. 






-------------------------------

Updated on 2021-09-25 at 17:48:29 +0900
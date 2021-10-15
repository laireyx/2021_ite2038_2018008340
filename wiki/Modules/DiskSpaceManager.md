

# DiskSpaceManager



## Namespaces

| Name           |
| -------------- |
| **[file_helper](/Namespaces/file_helper)** <br>Filemanager helper.  |
| **[page_helper](/Namespaces/page_helper)** <br><a href="/Classes/Page">Page</a> helper.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[TableInstance](/Classes/TableInstance)** <br>Table file instance.  |
| class | **[Page](/Classes/Page)** <br>struct for abstract page.  |
| class | **[HeaderPage](/Classes/HeaderPage)** <br>struct for the header page.  |
| class | **[FreePage](/Classes/FreePage)** <br>struct for the free page.  |
| class | **[PageHeader](/Classes/PageHeader)** <br>page header for allocated(internal and leaf) node.  |
| class | **[PageSlot](/Classes/PageSlot)** <br>page slot for allocated(internal and leaf) node.  |
| class | **[PageBranch](/Classes/PageBranch)** <br>page slot for internal node.  |
| class | **[AllocatedPage](/Classes/AllocatedPage)** <br>struct for allocated page.  |
| class | **[AllocatedFullPage](/Classes/AllocatedFullPage)** <br>struct for any allocated page.  |
| class | **[InternalPage](/Classes/InternalPage)** <br>struct for allocated internal page.  |
| class | **[LeafPage](/Classes/LeafPage)** <br>struct for allocated leaf page.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef struct <a href="/Classes/TableInstance">TableInstance</a> | **[TableInstance](/Modules/DiskSpaceManager#typedef-tableinstance)**  |
| typedef <a href="/Classes/Page">Page</a> | **[page_t](/Modules/DiskSpaceManager#typedef-page_t)**  |
| typedef <a href="/Classes/PageHeader">PageHeader</a> | **[pageheader_t](/Modules/DiskSpaceManager#typedef-pageheader_t)**  |
| typedef <a href="/Classes/HeaderPage">HeaderPage</a> | **[headerpage_t](/Modules/DiskSpaceManager#typedef-headerpage_t)**  |
| typedef <a href="/Classes/FreePage">FreePage</a> | **[freepage_t](/Modules/DiskSpaceManager#typedef-freepage_t)**  |
| typedef <a href="/Classes/AllocatedFullPage">AllocatedFullPage</a> | **[allocatedpage_t](/Modules/DiskSpaceManager#typedef-allocatedpage_t)**  |
| typedef <a href="/Classes/InternalPage">InternalPage</a> | **[internalpage_t](/Modules/DiskSpaceManager#typedef-internalpage_t)**  |
| typedef <a href="/Classes/LeafPage">LeafPage</a> | **[leafpage_t](/Modules/DiskSpaceManager#typedef-leafpage_t)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| tableid_t | **[file_open_table_file](/Modules/DiskSpaceManager#function-file_open_table_file)**(const char * path)<br>Open existing table file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Modules/DiskSpaceManager#function-file_alloc_page)**(int64_t table_id)<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Modules/DiskSpaceManager#function-file_free_page)**(int64_t table_id, pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Modules/DiskSpaceManager#function-file_read_page)**(int64_t table_id, pagenum_t pagenum, <a href="/Modules/DiskSpaceManager#typedef-page-t">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Modules/DiskSpaceManager#function-file_write_page)**(int64_t table_id, pagenum_t pagenum, const <a href="/Modules/DiskSpaceManager#typedef-page-t">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_table_files](/Modules/DiskSpaceManager#function-file_close_table_files)**()<br>Stop referencing the table files.  |
| struct <a href="/Classes/PageSlot">PageSlot</a> | **[__attribute__](/Modules/DiskSpaceManager#function-__attribute__)**((packed) ) |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[INITIAL_TABLE_FILE_SIZE](/Modules/DiskSpaceManager#variable-initial_table_file_size)** <br>Initial size(in bytes) of newly created table file.  |
| constexpr int | **[MAX_TABLE_INSTANCE](/Modules/DiskSpaceManager#variable-max_table_instance)** <br>Maximum number of table instances count.  |
| constexpr int | **[INITIAL_TABLE_CAPS](/Modules/DiskSpaceManager#variable-initial_table_caps)** <br>Initial number of page count in newly created table file.  |
| constexpr int | **[PAGE_SIZE](/Modules/DiskSpaceManager#variable-page_size)** <br>Size of each page(in bytes).  |
| constexpr int | **[PAGE_HEADER_SIZE](/Modules/DiskSpaceManager#variable-page_header_size)** <br>Size of page header(in bytes).  |
| constexpr int | **[MAX_PAGE_BRANCHES](/Modules/DiskSpaceManager#variable-max_page_branches)** <br>Maximum number of page branches.  |
| struct <a href="/Classes/PageBranch">PageBranch</a> | **[__attribute__](/Modules/DiskSpaceManager#variable-__attribute__)**  |
| int | **[table_instance_count](/Modules/DiskSpaceManager#variable-table_instance_count)** <br>current table instance number  |
| <a href="/Classes/TableInstance">TableInstance</a> | **[table_instances](/Modules/DiskSpaceManager#variable-table_instances)** <br>all table instances  |

## Types Documentation

### typedef TableInstance

```
typedef struct TableInstance TableInstance;
```


### typedef page_t

```
typedef Page page_t;
```


### typedef pageheader_t

```
typedef PageHeader pageheader_t;
```


### typedef headerpage_t

```
typedef HeaderPage headerpage_t;
```


### typedef freepage_t

```
typedef FreePage freepage_t;
```


### typedef allocatedpage_t

```
typedef AllocatedFullPage allocatedpage_t;
```


### typedef internalpage_t

```
typedef InternalPage internalpage_t;
```


### typedef leafpage_t

```
typedef LeafPage leafpage_t;
```



## Functions Documentation

### function file_open_table_file

```
tableid_t file_open_table_file(
    const char * path
)
```

Open existing table file or create one if not existed. 

**Parameters**: 

  * **path** Table file path. 


**Return**: ID of the opened table file. 

### function file_alloc_page

```
pagenum_t file_alloc_page(
    int64_t table_id
)
```

Allocate an on-disk page from the free page list. 

**Parameters**: 

  * **table_id** table id obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-table-file">file&#95;open&#95;table&#95;file()</a></code>. 


**Return**: >0 <a href="/Classes/Page">Page</a> index number if allocation success. 0 Zero if allocation failed. 

### function file_free_page

```
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

```
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

```
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

```
void file_close_table_files()
```

Stop referencing the table files. 

### function __attribute__

```
struct PageSlot __attribute__(
    (packed) 
)
```



## Attributes Documentation

### variable INITIAL_TABLE_FILE_SIZE

```
constexpr int INITIAL_TABLE_FILE_SIZE = 10 * 1024 * 1024;
```

Initial size(in bytes) of newly created table file. 

It means 10MiB. 


### variable MAX_TABLE_INSTANCE

```
constexpr int MAX_TABLE_INSTANCE = 32;
```

Maximum number of table instances count. 

### variable INITIAL_TABLE_CAPS

```
constexpr int INITIAL_TABLE_CAPS =
    INITIAL_TABLE_FILE_SIZE / MAX_TABLE_INSTANCE;
```

Initial number of page count in newly created table file. 

Its value is 2560. 


### variable PAGE_SIZE

```
constexpr int PAGE_SIZE = 4096;
```

Size of each page(in bytes). 

### variable PAGE_HEADER_SIZE

```
constexpr int PAGE_HEADER_SIZE = 128;
```

Size of page header(in bytes). 

### variable MAX_PAGE_BRANCHES

```
constexpr int MAX_PAGE_BRANCHES = 248;
```

Maximum number of page branches. 

### variable __attribute__

```
struct PageBranch __attribute__;
```


### variable table_instance_count

```
int table_instance_count = 0;
```

current table instance number 

### variable table_instances

```
TableInstance table_instances;
```

all table instances 




-------------------------------

Updated on 2021-10-16 at 00:32:30 +0900
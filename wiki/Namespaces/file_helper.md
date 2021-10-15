

# file_helper

**Module:** **[DiskSpaceManager](/Modules/DiskSpaceManager)**

Filemanager helper.  [More...](#detailed-description)

## Functions

|                | Name           |
| -------------- | -------------- |
| <a href="/Classes/TableInstance">TableInstance</a> & | **[get_table](/Namespaces/file_helper#function-get_table)**(tableid_t table_id)<br>Get table instance.  |
| void | **[extend_capacity](/Namespaces/file_helper#function-extend_capacity)**(tableid_t table_id, pagenum_t newsize)<br>Automatically check and size-up a page file.  |
| void | **[flush_header](/Namespaces/file_helper#function-flush_header)**(tableid_t table_id)<br>Flush a header page as "pagenum 0".  |

## Detailed Description

Filemanager helper. 

This namespace includes some helper functions which are used by filemanager API. Those functions are not a part of filemanager API, but frequently used part of it so I wrapped these functions. 


## Functions Documentation

### function get_table

```cpp
TableInstance & get_table(
    tableid_t table_id
)
```

Get table instance. 

**Parameters**: 

  * **table_id** Target table id 


Get the reference of the table instance ojbect corresponds with the given table id.


### function extend_capacity

```cpp
void extend_capacity(
    tableid_t table_id,
    pagenum_t newsize
)
```

Automatically check and size-up a page file. 

**Parameters**: 

  * **table_id** Target table id. 
  * **newsize** extended size. default is 0, which means doubling the reserved page count if there are no free page. 


If <code>newsize &gt; page&#95;num</code>, reserve pages so that total page num is equivalent to newsize. If <code>newsize = 0</code> and header page's free_page_idx is 0, double the reserved page count.


next free page index is next page index, unless it is the last page.

next free page index is next page index, unless it is the last page.


### function flush_header

```cpp
void flush_header(
    tableid_t table_id
)
```

Flush a header page as "pagenum 0". 

Write header page into offset 0 of the current table file 






-------------------------------

Updated on 2021-10-15 at 15:45:30 +0900
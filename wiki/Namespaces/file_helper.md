

# file_helper

**Module:** **[DiskSpaceManager](/Modules/DiskSpaceManager)**

Filemanager helper.  [More...](#detailed-description)

## Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[switch_to_fd](/Namespaces/file_helper#function-switch_to_fd)**(int fd)<br>Switch current database into given database.  |
| void | **[extend_capacity](/Namespaces/file_helper#function-extend_capacity)**(pagenum_t newsize)<br>Automatically check and size-up a page file.  |
| void | **[flush_header](/Namespaces/file_helper#function-flush_header)**()<br>Flush a header page as "pagenum 0".  |

## Detailed Description

Filemanager helper. 

This namespace includes some helper functions which are used by filemanager API. Those functions are not a part of filemanager API, but frequently used part of it so I wrapped these functions. 


## Functions Documentation

### function switch_to_fd

```cpp
bool switch_to_fd(
    int fd
)
```

Switch current database into given database. 

**Parameters**: 

  * **fd** Database file descriptor obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-database-file">file&#95;open&#95;database&#95;file()</a></code>. 


If current <code>database&#95;fd == fd</code>, then do nothing. If not, change database_fd to given fd and re-read header_page from it.


### function extend_capacity

```cpp
void extend_capacity(
    pagenum_t newsize
)
```

Automatically check and size-up a page file. 

**Parameters**: 

  * **newsize** extended size. default is 0, which means doubling the reserved page count if there are no free page. 


If <code>newsize &gt; page&#95;num</code>, reserve pages so that total page num is equivalent to newsize. If <code>newsize = 0</code> and header page's free_page_idx is 0, double the reserved page count.


next free page index is next page index, unless it is the last page.

next free page index is next page index, unless it is the last page.


### function flush_header

```cpp
void flush_header()
```

Flush a header page as "pagenum 0". 

Write header page into offset 0 of the current database file descriptor. 






-------------------------------

Updated on 2021-10-01 at 19:55:33 +0900


# DiskSpaceManager



## Namespaces

| Name           |
| -------------- |
| **[file_helper](/Namespaces/file_helper)** <br>Filemanager helper.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[Page](/Classes/Page)** <br>struct for abstract page.  |
| class | **[AllocatedPage](/Classes/AllocatedPage)** <br>struct for allocated page.  |
| class | **[HeaderPage](/Classes/HeaderPage)** <br>struct for the header page.  |
| class | **[FreePage](/Classes/FreePage)** <br>struct for the free page.  |

## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[file_open_database_file](/Modules/DiskSpaceManager#function-file_open_database_file)**(const char * path)<br>Open existing database file or create one if not existed.  |
| pagenum_t | **[file_alloc_page](/Modules/DiskSpaceManager#function-file_alloc_page)**(int fd)<br>Allocate an on-disk page from the free page list.  |
| void | **[file_free_page](/Modules/DiskSpaceManager#function-file_free_page)**(int fd, pagenum_t pagenum)<br>Free an on-disk page to the free page list.  |
| void | **[file_read_page](/Modules/DiskSpaceManager#function-file_read_page)**(int fd, pagenum_t pagenum, <a href="/Classes/Page">page_t</a> * dest)<br>Read an on-disk page into the in-memory page structure(dest)  |
| void | **[file_write_page](/Modules/DiskSpaceManager#function-file_write_page)**(int fd, pagenum_t pagenum, const <a href="/Classes/Page">page_t</a> * src)<br>Write an in-memory page(src) to the on-disk page.  |
| void | **[file_close_database_file](/Modules/DiskSpaceManager#function-file_close_database_file)**()<br>Stop referencing the database file.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[INITIAL_DB_FILE_SIZE](/Modules/DiskSpaceManager#variable-initial_db_file_size)** <br>Initial size(in bytes) of newly created database file.  |
| constexpr int | **[MAX_DATABASE_INSTANCE](/Modules/DiskSpaceManager#variable-max_database_instance)** <br>Maximum number of database instances count.  |
| constexpr int | **[INITIAL_DATABASE_CAPS](/Modules/DiskSpaceManager#variable-initial_database_caps)** <br>Initial number of page count in newly created database file.  |
| constexpr int | **[PAGE_SIZE](/Modules/DiskSpaceManager#variable-page_size)** <br>Size of each page(in bytes).  |
| int | **[database_instance_count](/Modules/DiskSpaceManager#variable-database_instance_count)** <br>current database instance number  |
| <a href="/Classes/DatabaseInstance">DatabaseInstance</a> | **[database_instances](/Modules/DiskSpaceManager#variable-database_instances)** <br>all database instances  |
| int | **[database_fd](/Modules/DiskSpaceManager#variable-database_fd)** <br>currently opened database file descriptor  |
| <a href="/Classes/HeaderPage">headerpage_t</a> | **[header_page](/Modules/DiskSpaceManager#variable-header_page)** <br>currently opened database header page  |


## Functions Documentation

### function file_open_database_file

```
int file_open_database_file(
    const char * path
)
```

Open existing database file or create one if not existed. 

**Parameters**: 

  * **path** Database file path. 


**Return**: ID of the opened database file. 

### function file_alloc_page

```
pagenum_t file_alloc_page(
    int fd
)
```

Allocate an on-disk page from the free page list. 

**Parameters**: 

  * **fd** Database file descriptor obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-database-file">file&#95;open&#95;database&#95;file()</a></code>. 


**Return**: >0 <a href="/Classes/Page">Page</a> index number if allocation success. 0 Zero if allocation failed. 

### function file_free_page

```
void file_free_page(
    int fd,
    pagenum_t pagenum
)
```

Free an on-disk page to the free page list. 

**Parameters**: 

  * **fd** Database file descriptor obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-database-file">file&#95;open&#95;database&#95;file()</a></code>. 
  * **pagenum** page index. 


### function file_read_page

```
void file_read_page(
    int fd,
    pagenum_t pagenum,
    page_t * dest
)
```

Read an on-disk page into the in-memory page structure(dest) 

**Parameters**: 

  * **fd** Database file descriptor obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-database-file">file&#95;open&#95;database&#95;file()</a></code>. 
  * **pagenum** page index. 
  * **dest** the pointer of the page data. 


### function file_write_page

```
void file_write_page(
    int fd,
    pagenum_t pagenum,
    const page_t * src
)
```

Write an in-memory page(src) to the on-disk page. 

**Parameters**: 

  * **fd** Database file descriptor obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-database-file">file&#95;open&#95;database&#95;file()</a></code>. 
  * **pagenum** page index. 
  * **src** the pointer of the page data. 


### function file_close_database_file

```
void file_close_database_file()
```

Stop referencing the database file. 


## Attributes Documentation

### variable INITIAL_DB_FILE_SIZE

```
constexpr int INITIAL_DB_FILE_SIZE = 10 * 1024 * 1024;
```

Initial size(in bytes) of newly created database file. 

It means 10MiB. 


### variable MAX_DATABASE_INSTANCE

```
constexpr int MAX_DATABASE_INSTANCE = 1024;
```

Maximum number of database instances count. 

### variable INITIAL_DATABASE_CAPS

```
constexpr int INITIAL_DATABASE_CAPS =
    INITIAL_DB_FILE_SIZE / MAX_DATABASE_INSTANCE;
```

Initial number of page count in newly created database file. 

Its value is 2560. 


### variable PAGE_SIZE

```
constexpr int PAGE_SIZE = 4096;
```

Size of each page(in bytes). 

### variable database_instance_count

```
int database_instance_count = 0;
```

current database instance number 

### variable database_instances

```
DatabaseInstance database_instances;
```

all database instances 

### variable database_fd

```
int database_fd = 0;
```

currently opened database file descriptor 

### variable header_page

```
headerpage_t header_page;
```

currently opened database header page 




-------------------------------

Updated on 2021-10-01 at 19:40:48 +0900


# TableInstance

**Module:** **[DiskSpaceManager](/Modules/based B+ treeSpaceManager)**



Table file instance. 


`#include <file.h>`

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| char * | **[file_path](/Classes/TableInstance#variable-file_path)** <br>Real table file path(obtained by realpath(3)).  |
| int | **[file_descriptor](/Classes/TableInstance#variable-file_descriptor)** <br>Table file descriptor.  |
| <a href="/Modules/based B+ treeSpaceManager#typedef-headerpage-t">headerpage_t</a> | **[header_page](/Classes/TableInstance#variable-header_page)** <br>Table header page.  |

## Public Attributes Documentation

### variable file_path

```cpp
char * file_path;
```

Real table file path(obtained by realpath(3)). 

### variable file_descriptor

```cpp
int file_descriptor;
```

Table file descriptor. 

### variable header_page

```cpp
headerpage_t header_page;
```

Table header page. 

-------------------------------

Updated on 2021-10-15 at 13:42:29 +0900
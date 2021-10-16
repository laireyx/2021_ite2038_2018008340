

# HeaderPage

**Module:** **[DiskSpaceManager](/Modules/DiskSpaceManager)**



struct for the header page. 


`#include <page.h>`

Inherits from [Page](/Classes/Page)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| pagenum_t | **[free_page_idx](/Classes/HeaderPage#variable-free_page_idx)** <br>The first free page index.  |
| uint64_t | **[page_num](/Classes/HeaderPage#variable-page_num)** <br>Total count of the page reserved.  |
| pagenum_t | **[root_page_idx](/Classes/HeaderPage#variable-root_page_idx)** <br>The root page index.  |
| uint8_t | **[reserved](/Classes/HeaderPage#variable-reserved)** <br>Reserved area for next project.  |

## Public Attributes Documentation

### variable free_page_idx

```cpp
pagenum_t free_page_idx;
```

The first free page index. 

### variable page_num

```cpp
uint64_t page_num;
```

Total count of the page reserved. 

### variable root_page_idx

```cpp
pagenum_t root_page_idx;
```

The root page index. 

### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for next project. 

-------------------------------

Updated on 2021-10-16 at 20:56:45 +0900
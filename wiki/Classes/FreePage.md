

# FreePage

**Module:** **[DiskSpaceManager](/Modules/DiskSpaceManager)**



struct for the free page. 


`#include <page.h>`

Inherits from [Page](/Classes/Page)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| pagenum_t | **[next_free_idx](/Classes/FreePage#variable-next_free_idx)** <br>Index of the very next free page.  |
| uint8_t | **[reserved](/Classes/FreePage#variable-reserved)** <br>Reserved area for next project.  |

## Public Attributes Documentation

### variable next_free_idx

```cpp
pagenum_t next_free_idx;
```

Index of the very next free page. 

### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for next project. 

-------------------------------

Updated on 2021-10-16 at 00:40:34 +0900
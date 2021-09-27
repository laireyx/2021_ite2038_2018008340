

# HeaderPage



struct for the header page. 


`#include <page.h>`

Inherits from [Page](/Classes/Page)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint64_t | **[free_page_idx](/Classes/HeaderPage#variable-free_page_idx)** <br>The first free page index.  |
| uint64_t | **[page_num](/Classes/HeaderPage#variable-page_num)** <br>Total count of the page reserved.  |
| uint8_t | **[reserved](/Classes/HeaderPage#variable-reserved)**  |

## Public Attributes Documentation

### variable free_page_idx

```cpp
uint64_t free_page_idx;
```

The first free page index. 

### variable page_num

```cpp
uint64_t page_num;
```

Total count of the page reserved. 

### variable reserved

```cpp
uint8_t reserved;
```


-------------------------------

Updated on 2021-09-27 at 20:57:40 +0900
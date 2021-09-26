---
title: HeaderPage
summary: struct for the header page. 

---

# HeaderPage



struct for the header page. 


`#include <page.h>`

Inherits from [Page](/Classes/structPage)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint64_t | **[free_page_idx](/Classes/structHeaderPage#variable-free-page-idx)** <br>The first free page index.  |
| uint64_t | **[page_num](/Classes/structHeaderPage#variable-page-num)** <br>Total count of the page reserved.  |
| uint8_t | **[reserved](/Classes/structHeaderPage#variable-reserved)**  |

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

Updated on 2021-09-26 at 23:34:22 +0900


# db/include/page.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[Page](/Classes/Page)** <br>struct for abstract page.  |
| class | **[AllocatedPage](/Classes/AllocatedPage)** <br>struct for allocated page.  |
| class | **[HeaderPage](/Classes/HeaderPage)** <br>struct for the header page.  |
| class | **[FreePage](/Classes/FreePage)** <br>struct for the free page.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[PAGE_SIZE](/Modules/DiskSpaceManager#variable-page_size)** <br>Size of each page(in bytes).  |



## Attributes Documentation

### variable PAGE_SIZE

```cpp
constexpr int PAGE_SIZE = 4096;
```

Size of each page(in bytes). 


## Source code

```cpp

#pragma once
#include <cstdint>

constexpr int PAGE_SIZE = 4096;

struct Page {};

struct AllocatedPage : public Page {
    uint8_t reserved[PAGE_SIZE];
};

struct HeaderPage : public Page {
    uint64_t free_page_idx;
    uint64_t page_num;

    uint8_t reserved[PAGE_SIZE - 16];
};

struct FreePage : public Page {
    uint64_t next_free_idx;

    uint8_t reserved[PAGE_SIZE - 8];
};
```


-------------------------------

Updated on 2021-10-01 at 23:25:36 +0900

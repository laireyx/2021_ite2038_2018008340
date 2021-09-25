---
title: filemanager/page.h

---

# filemanager/page.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[Page](/Classes/structPage)** <br>struct for abstract page.  |
| class | **[AllocatedPage](/Classes/structAllocatedPage)** <br>struct for allocated page.  |
| class | **[HeaderPage](/Classes/structHeaderPage)** <br>struct for the header page.  |
| class | **[FreePage](/Classes/structFreePage)** <br>struct for the free page.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[PAGE_SIZE](/Files/page_8h#variable-page-size)** <br>Size of each page(in bytes).  |



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

struct Page { };

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

Updated on 2021-09-26 at 01:16:16 +0900

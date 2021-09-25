---
title: filemanager/page.h

---

# filemanager/page.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[Page](Classes/structPage.md)** <br>struct for abstract page.  |
| class | **[AllocatedPage](Classes/structAllocatedPage.md)** <br>struct for allocated page.  |
| class | **[HeaderPage](Classes/structHeaderPage.md)** <br>struct for the header page.  |
| class | **[FreePage](Classes/structFreePage.md)** <br>struct for the free page.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[PAGE_SIZE](Files/page_8h.md#variable-page-size)**  |



## Attributes Documentation

### variable PAGE_SIZE

```cpp
constexpr int PAGE_SIZE = 4096;
```



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

Updated on 2021-09-25 at 19:30:23 +0900

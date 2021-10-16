

# db/include/page.h



## Namespaces

| Name           |
| -------------- |
| **[page_helper](/Namespaces/page_helper)** <br><a href="/Classes/Page">Page</a> helper.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[Page](/Classes/Page)** <br>struct for abstract page.  |
| class | **[HeaderPage](/Classes/HeaderPage)** <br>struct for the header page.  |
| class | **[FreePage](/Classes/FreePage)** <br>struct for the free page.  |
| class | **[PageHeader](/Classes/PageHeader)** <br>page header for allocated(internal and leaf) node.  |
| struct | **[PageHeader::ReservedFooter](/Classes/PageHeader_1_1ReservedFooter)**  |
| class | **[PageSlot](/Classes/PageSlot)** <br>page slot for allocated(internal and leaf) node.  |
| class | **[PageBranch](/Classes/PageBranch)** <br>page slot for internal node.  |
| class | **[AllocatedPage](/Classes/AllocatedPage)** <br>struct for allocated page.  |
| class | **[AllocatedFullPage](/Classes/AllocatedFullPage)** <br>struct for any allocated page.  |
| class | **[InternalPage](/Classes/InternalPage)** <br>struct for allocated internal page.  |
| class | **[LeafPage](/Classes/LeafPage)** <br>struct for allocated leaf page.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef <a href="/Classes/Page">Page</a> | **[page_t](/Modules/DiskSpaceManager#typedef-page_t)**  |
| typedef <a href="/Classes/PageHeader">PageHeader</a> | **[pageheader_t](/Modules/DiskSpaceManager#typedef-pageheader_t)**  |
| typedef <a href="/Classes/HeaderPage">HeaderPage</a> | **[headerpage_t](/Modules/DiskSpaceManager#typedef-headerpage_t)**  |
| typedef <a href="/Classes/FreePage">FreePage</a> | **[freepage_t](/Modules/DiskSpaceManager#typedef-freepage_t)**  |
| typedef <a href="/Classes/AllocatedFullPage">AllocatedFullPage</a> | **[allocatedpage_t](/Modules/DiskSpaceManager#typedef-allocatedpage_t)**  |
| typedef <a href="/Classes/InternalPage">InternalPage</a> | **[internalpage_t](/Modules/DiskSpaceManager#typedef-internalpage_t)**  |
| typedef <a href="/Classes/LeafPage">LeafPage</a> | **[leafpage_t](/Modules/DiskSpaceManager#typedef-leafpage_t)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| struct <a href="/Classes/PageSlot">PageSlot</a> | **[__attribute__](/Modules/DiskSpaceManager#function-__attribute__)**((packed) ) |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[PAGE_SIZE](/Modules/DiskSpaceManager#variable-page_size)** <br>Size of each page(in bytes).  |
| constexpr int | **[PAGE_HEADER_SIZE](/Modules/DiskSpaceManager#variable-page_header_size)** <br>Size of page header(in bytes).  |
| constexpr int | **[MAX_PAGE_BRANCHES](/Modules/DiskSpaceManager#variable-max_page_branches)** <br>Maximum number of page branches.  |
| int64_t | **[key](/Files/db/include/page.h#variable-key)** <br>The page key.  |
| uint16_t | **[value_size](/Files/db/include/page.h#variable-value_size)** <br>The value size(in bytes).  |
| uint16_t | **[value_offset](/Files/db/include/page.h#variable-value_offset)** <br>The value offset(in bytes).  |
| struct <a href="/Classes/PageBranch">PageBranch</a> | **[__attribute__](/Modules/DiskSpaceManager#variable-__attribute__)**  |
| uint8_t | **[reserved](/Files/db/include/page.h#variable-reserved)** <br>Reserved area for normal allocated page.  |

## Types Documentation

### typedef page_t

```cpp
typedef Page page_t;
```


### typedef pageheader_t

```cpp
typedef PageHeader pageheader_t;
```


### typedef headerpage_t

```cpp
typedef HeaderPage headerpage_t;
```


### typedef freepage_t

```cpp
typedef FreePage freepage_t;
```


### typedef allocatedpage_t

```cpp
typedef AllocatedFullPage allocatedpage_t;
```


### typedef internalpage_t

```cpp
typedef InternalPage internalpage_t;
```


### typedef leafpage_t

```cpp
typedef LeafPage leafpage_t;
```



## Functions Documentation

### function __attribute__

```cpp
struct PageSlot __attribute__(
    (packed) 
)
```



## Attributes Documentation

### variable PAGE_SIZE

```cpp
constexpr int PAGE_SIZE = 4096;
```

Size of each page(in bytes). 

### variable PAGE_HEADER_SIZE

```cpp
constexpr int PAGE_HEADER_SIZE = 128;
```

Size of page header(in bytes). 

### variable MAX_PAGE_BRANCHES

```cpp
constexpr int MAX_PAGE_BRANCHES = 248;
```

Maximum number of page branches. 

### variable key

```cpp
int64_t key;
```

The page key. 

### variable value_size

```cpp
uint16_t value_size;
```

The value size(in bytes). 

### variable value_offset

```cpp
uint16_t value_offset;
```

The value offset(in bytes). 

### variable __attribute__

```cpp
struct PageBranch __attribute__;
```


### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for normal allocated page. 


## Source code

```cpp

#pragma once
#include "types.h"

#include <cstdint>

constexpr int PAGE_SIZE = 4096;

constexpr int PAGE_HEADER_SIZE = 128;

constexpr int MAX_PAGE_BRANCHES = 248;

struct Page {};

struct HeaderPage : public Page {
    pagenum_t free_page_idx;
    uint64_t page_num;
    pagenum_t root_page_idx;

    uint8_t reserved[PAGE_SIZE - 24];
};

struct FreePage : public Page {
    pagenum_t next_free_idx;

    uint8_t reserved[PAGE_SIZE - 8];
};

struct PageHeader {
    pagenum_t parent_page_idx;
    uint32_t is_leaf_page;
    uint32_t key_num;

    uint8_t reserved[PAGE_HEADER_SIZE - 16 - 16];

    struct ReservedFooter {
        uint64_t footer_1;
        uint64_t footer_2;
    } reserved_footer;
};

struct PageSlot {
    int64_t key;
    uint16_t value_size;
    uint16_t value_offset;
} __attribute__((packed));

struct PageBranch {
    int64_t key;
    uint16_t page_idx;
};

struct AllocatedPage : public Page {
    PageHeader page_header;
};

struct AllocatedFullPage : public AllocatedPage {
    uint8_t reserved[PAGE_SIZE - PAGE_HEADER_SIZE];
};

struct InternalPage : public AllocatedPage {

    PageBranch page_branches[MAX_PAGE_BRANCHES];
};

struct LeafPage : public AllocatedPage {

    uint8_t reserved[PAGE_SIZE - PAGE_HEADER_SIZE];
} __attribute__((packed));

namespace page_helper {

PageSlot* get_page_slot(LeafPage* page);
void get_leaf_value(LeafPage* page, int value_idx, char* value,
                    uint16_t* value_size = nullptr);
void get_leaf_value(LeafPage* page, uint16_t value_offset, uint16_t value_size,
                    char* value);
bool has_enough_space(LeafPage* page, uint16_t value_size);
uint64_t* get_free_space(LeafPage* page);
pagenum_t* get_sibling_idx(LeafPage* page);

bool add_leaf_value(LeafPage* page, int64_t key, const char* value,
                    uint16_t value_size);
bool remove_leaf_value(LeafPage* page, int64_t key);

bool add_internal_key(InternalPage* page, int64_t key, pagenum_t page_idx);
bool remove_internal_key(InternalPage* page, int64_t key);
pagenum_t* get_leftmost_child_idx(InternalPage* page);
}

typedef Page page_t;
typedef PageHeader pageheader_t;
typedef HeaderPage headerpage_t;
typedef FreePage freepage_t;
typedef AllocatedFullPage allocatedpage_t;
typedef InternalPage internalpage_t;
typedef LeafPage leafpage_t;
```


-------------------------------

Updated on 2021-10-16 at 00:40:34 +0900

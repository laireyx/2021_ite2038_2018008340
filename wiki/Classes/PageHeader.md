

# PageHeader

**Module:** **[DiskSpaceManager](/Modules/based B+ treeSpaceManager)**



page header for allocated(internal and leaf) node. 


`#include <page.h>`

## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[ReservedFooter](/Classes/PageHeader_1_1ReservedFooter)**  |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| pagenum_t | **[parent_page_idx](/Classes/PageHeader#variable-parent_page_idx)** <br>The parent page index.  |
| uint32_t | **[is_leaf_page](/Classes/PageHeader#variable-is_leaf_page)** <br>1 if this page is leaf page, and 0 if internal page.  |
| uint32_t | **[key_num](/Classes/PageHeader#variable-key_num)** <br>Number of keys this page is holding.  |
| uint8_t | **[reserved](/Classes/PageHeader#variable-reserved)** <br>Reserved area for page header.  |
| struct <a href="/Classes/PageHeader_1_1ReservedFooter">PageHeader::ReservedFooter</a> | **[reserved_footer](/Classes/PageHeader#variable-reserved_footer)**  |

## Public Attributes Documentation

### variable parent_page_idx

```cpp
pagenum_t parent_page_idx;
```

The parent page index. 

### variable is_leaf_page

```cpp
uint32_t is_leaf_page;
```

1 if this page is leaf page, and 0 if internal page. 

### variable key_num

```cpp
uint32_t key_num;
```

Number of keys this page is holding. 

### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for page header. 

### variable reserved_footer

```cpp
struct PageHeader::ReservedFooter reserved_footer;
```


-------------------------------

Updated on 2021-10-15 at 13:42:29 +0900
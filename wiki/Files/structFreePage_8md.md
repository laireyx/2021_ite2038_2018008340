---
title: structFreePage.md

---

# structFreePage.md






## Source code

```markdown
---
title: FreePage
summary: struct for the free page. 

---

# FreePage

**Module:** **[Page structure](Modules/group__Page.md)**



struct for the free page. 


`#include <page.h>`

Inherits from [Page](Classes/structPage.md)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint64_t | **[next_free_idx](Classes/structFreePage.md#variable-next-free-idx)** <br>Index of the very next free page.  |
| uint8_t | **[reserved](Classes/structFreePage.md#variable-reserved)** <br>Reserved area for next project.  |

## Public Attributes Documentation

### variable next_free_idx

```cpp
uint64_t next_free_idx;
```

Index of the very next free page. 

### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for next project. 

-------------------------------

Updated on 2021-09-25 at 17:41:34 +0900
```


-------------------------------

Updated on 2021-09-25 at 17:48:29 +0900

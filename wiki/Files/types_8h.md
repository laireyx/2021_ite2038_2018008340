---
title: filemanager/types.h

---

# filemanager/types.h



## Types

|                | Name           |
| -------------- | -------------- |
| typedef uint64_t | **[pagenum_t](/Files/types_8h#typedef-pagenum-t)**  |
| typedef [Page](/Classes/structPage) | **[page_t](/Files/types_8h#typedef-page-t)**  |
| typedef [AllocatedPage](/Classes/structAllocatedPage) | **[allocatedpage_t](/Files/types_8h#typedef-allocatedpage-t)**  |
| typedef [HeaderPage](/Classes/structHeaderPage) | **[headerpage_t](/Files/types_8h#typedef-headerpage-t)**  |
| typedef [FreePage](/Classes/structFreePage) | **[freepage_t](/Files/types_8h#typedef-freepage-t)**  |

## Types Documentation

### typedef pagenum_t

```cpp
typedef uint64_t pagenum_t;
```


### typedef page_t

```cpp
typedef Page page_t;
```


### typedef allocatedpage_t

```cpp
typedef AllocatedPage allocatedpage_t;
```


### typedef headerpage_t

```cpp
typedef HeaderPage headerpage_t;
```


### typedef freepage_t

```cpp
typedef FreePage freepage_t;
```





## Source code

```cpp
#pragma once

#include <cstdint>

typedef uint64_t pagenum_t;

#include "page.h"

typedef Page page_t;
typedef AllocatedPage allocatedpage_t;
typedef HeaderPage headerpage_t;
typedef FreePage freepage_t;
```


-------------------------------

Updated on 2021-09-26 at 01:11:28 +0900

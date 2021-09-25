---
title: types_8h.md

---

# types_8h.md






## Source code

```markdown
---
title: types.h

---

# types.h



## Types

|                | Name           |
| -------------- | -------------- |
| typedef uint64_t | **[pagenum_t](Modules/group__Type.md#typedef-pagenum-t)**  |
| typedef [Page](Classes/structPage.md) | **[page_t](Modules/group__Type.md#typedef-page-t)**  |
| typedef [AllocatedPage](Classes/structAllocatedPage.md) | **[allocatedpage_t](Modules/group__Type.md#typedef-allocatedpage-t)**  |
| typedef [HeaderPage](Classes/structHeaderPage.md) | **[headerpage_t](Modules/group__Type.md#typedef-headerpage-t)**  |
| typedef [FreePage](Classes/structFreePage.md) | **[freepage_t](Modules/group__Type.md#typedef-freepage-t)**  |

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

Updated on 2021-09-25 at 17:41:34 +0900
```


-------------------------------

Updated on 2021-09-25 at 17:48:29 +0900

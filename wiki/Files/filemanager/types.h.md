

# filemanager/types.h



## Types

|                | Name           |
| -------------- | -------------- |
| typedef uint64_t | **[pagenum_t](/Files/filemanager/types.h#typedef-pagenum_t)**  |
| typedef <a href="/Classes/Page">Page</a> | **[page_t](/Files/filemanager/types.h#typedef-page_t)**  |
| typedef <a href="/Classes/AllocatedPage">AllocatedPage</a> | **[allocatedpage_t](/Files/filemanager/types.h#typedef-allocatedpage_t)**  |
| typedef <a href="/Classes/HeaderPage">HeaderPage</a> | **[headerpage_t](/Files/filemanager/types.h#typedef-headerpage_t)**  |
| typedef <a href="/Classes/FreePage">FreePage</a> | **[freepage_t](/Files/filemanager/types.h#typedef-freepage_t)**  |

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

Updated on 2021-09-28 at 10:05:21 +0900

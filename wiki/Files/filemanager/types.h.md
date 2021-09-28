

# filemanager/types.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[DatabaseInstance](/Classes/DatabaseInstance)** <br>Database file instance.  |

## Types

|                | Name           |
| -------------- | -------------- |
| typedef uint64_t | **[pagenum_t](/Files/filemanager/types.h#typedef-pagenum_t)**  |
| typedef <a href="/Classes/Page">Page</a> | **[page_t](/Files/filemanager/types.h#typedef-page_t)**  |
| typedef <a href="/Classes/AllocatedPage">AllocatedPage</a> | **[allocatedpage_t](/Files/filemanager/types.h#typedef-allocatedpage_t)**  |
| typedef <a href="/Classes/HeaderPage">HeaderPage</a> | **[headerpage_t](/Files/filemanager/types.h#typedef-headerpage_t)**  |
| typedef <a href="/Classes/FreePage">FreePage</a> | **[freepage_t](/Files/filemanager/types.h#typedef-freepage_t)**  |
| typedef struct <a href="/Classes/DatabaseInstance">DatabaseInstance</a> | **[DatabaseInstance](/Files/filemanager/types.h#typedef-databaseinstance)**  |

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


### typedef DatabaseInstance

```cpp
typedef struct DatabaseInstance DatabaseInstance;
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

typedef struct DatabaseInstance {
    char* file_path;
    int file_descriptor;
} DatabaseInstance;
```


-------------------------------

Updated on 2021-09-29 at 00:54:48 +0900

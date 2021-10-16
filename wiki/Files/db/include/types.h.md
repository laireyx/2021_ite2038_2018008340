

# db/include/types.h



## Types

|                | Name           |
| -------------- | -------------- |
| typedef uint64_t | **[pagenum_t](/Files/db/include/types.h#typedef-pagenum_t)**  |
| typedef int64_t | **[tableid_t](/Files/db/include/types.h#typedef-tableid_t)**  |

## Types Documentation

### typedef pagenum_t

```cpp
typedef uint64_t pagenum_t;
```


### typedef tableid_t

```cpp
typedef int64_t tableid_t;
```





## Source code

```cpp
#pragma once

#include <cstdint>
#include <memory>

typedef uint64_t pagenum_t;
typedef int64_t tableid_t;
```


-------------------------------

Updated on 2021-10-16 at 21:37:11 +0900



# filemanager/errors.h



## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[_perrno](/Files/filemanager/errors.h#function-_perrno)**(bool exit_flag =false)<br>Print error message of errno and exit if flag is set.  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[CHECK](/Files/filemanager/errors.h#define-check)**(value) <br>If value < 0, then print error message.  |


## Functions Documentation

### function _perrno

```cpp
inline int _perrno(
    bool exit_flag =false
)
```

Print error message of errno and exit if flag is set. 

**Return**: -errno for return _perrno(); in some functions. 

If exit_flag = false, just print errno message. If exit_flag = true, print errno message and exit.




## Macros Documentation

### define CHECK

```cpp
#define CHECK(
    value
)
        do { \
        if((value) < 0) { \
            _perrno(); \
        } \
    } while(0)
```

If value < 0, then print error message. 

## Source code

```cpp
#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

inline int _perrno(bool exit_flag = false) {
    perror(strerror(errno));
    
    if(exit_flag) {
        exit(1);
    }

    return -errno;
}

#define CHECK(value) do { \
        if((value) < 0) { \
            _perrno(); \
        } \
    } while(0)
```


-------------------------------

Updated on 2021-09-29 at 00:31:01 +0900

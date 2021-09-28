

# filemanager/errors.h



## Namespaces

| Name           |
| -------------- |
| **[error](/Namespaces/error)** <br>Error.  |




## Source code

```cpp
#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

namespace error {
    inline int print(bool exit_flag = false) {
        perror(strerror(errno));
        
        if(exit_flag) {
            exit(1);
        }

        return -errno;
    }

    inline bool check(int value, bool exit_flag = false) {
        if(value < 0) {
            print(exit_flag);
            return false;
        }
        return true;
    }
}
```


-------------------------------

Updated on 2021-09-29 at 01:15:36 +0900

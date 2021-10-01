

# db/include/errors.h



## Namespaces

| Name           |
| -------------- |
| **[error](/Namespaces/error)** <br>Error.  |




## Source code

```cpp
#pragma once

#include <errno.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace error {

inline int print(bool exit_flag = false) {
    perror(strerror(errno));

    if (exit_flag) {
        exit(1);
    }

    return -errno;
}

inline bool ok(int value, bool exit_flag = false) {
    if (value < 0) {
        print(exit_flag);
        return false;
    }
    return true;
}

}  // namespace error
```


-------------------------------

Updated on 2021-10-01 at 18:39:50 +0900

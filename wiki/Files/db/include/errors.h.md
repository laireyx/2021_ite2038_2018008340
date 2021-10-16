

# db/include/errors.h



## Namespaces

| Name           |
| -------------- |
| **[error](/Namespaces/error)** <br>Error.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[error::Exception](/Classes/error::Exception)**  |




## Source code

```cpp
#pragma once

#include <errno.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace error {

constexpr int SILENT            = 0x0001;
constexpr int PRINT_ERROR       = 0x0002;
constexpr int THROW_EXCEPTION   = 0x0004;
constexpr int EXIT_PROGRAM      = 0x0008;

class Exception {
private:
    std::string error_message;
    
    Exception(const Exception& rhs);
    Exception& operator=(const Exception&);
public:
    Exception(std::string message) : error_message(message) { }
    ~Exception() { }
};

inline int print(bool exit_flag = PRINT_ERROR) {
    if(exit_flag & SILENT) {
        return -errno;
    }

    if(exit_flag & PRINT_ERROR) {
        perror("");
    }
    if(exit_flag & THROW_EXCEPTION) {
        throw new Exception("Error");
    }
    if(exit_flag & EXIT_PROGRAM) {
        exit(1);
    }

    return -errno;
}

inline bool ok(bool assertion, bool exit_flag = THROW_EXCEPTION) {
    if (!assertion) {
        if(exit_flag & SILENT) {
            return false;
        }
        if(exit_flag & PRINT_ERROR) {
            print(exit_flag);
        }
        if(exit_flag & THROW_EXCEPTION) {
            throw new Exception("Error");
        }
        if(exit_flag & EXIT_PROGRAM) {
            exit(1);
        }
        return false;
    }
    return true;
}

}  // namespace error
```


-------------------------------

Updated on 2021-10-16 at 21:45:02 +0900

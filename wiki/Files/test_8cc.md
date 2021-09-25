---
title: filemanager/test.cc

---

# filemanager/test.cc



## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[main](/Files/test_8cc#function-main)**() |


## Functions Documentation

### function main

```cpp
int main()
```




## Source code

```cpp
#include <iostream>
#include "file.h"

int main() {
    file_open_database_file("db");
    
    std::cout << file_alloc_page() << std::endl;
    std::cout << file_alloc_page() << std::endl;
    std::cout << file_alloc_page() << std::endl;
    std::cout << file_alloc_page() << std::endl;

    file_free_page(4);
    file_free_page(3);
    file_free_page(2);
    file_free_page(1);
    
    file_close_database_file();
    return 0;
}
```


-------------------------------

Updated on 2021-09-26 at 01:11:28 +0900

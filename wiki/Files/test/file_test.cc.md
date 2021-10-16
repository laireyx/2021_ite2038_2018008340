

# test/file_test.cc



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[BasicFileManagerTest](/Classes/BasicFileManagerTest)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , HandlesInitialization )<br>Tests file open/close APIs.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , HandlesPageAllocation )<br>Tests page allocation and free.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , CheckReadWriteOperation )<br>Tests page read/write operations.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , UniqueIdTest )<br>Tests unique table fd.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , SequentialAllocateTest )<br>Tests sequential allocation.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , RandomAllocateTest )<br>Tests random allocation.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[test_count](/Modules/TestCode#variable-test_count)**  |
| const char * | **[TABLE_PATH](/Modules/TestCode#variable-table_path)**  |
| const char * | **[TABLE_PATH_ALIAS](/Modules/TestCode#variable-table_path_alias)**  |
| const char * | **[ANOTHER_TABLE_PATH](/Modules/TestCode#variable-another_table_path)**  |


## Functions Documentation

### function TEST_F

```cpp
TEST_F(
    BasicFileManagerTest ,
    HandlesInitialization 
)
```

Tests file open/close APIs. 



1. Open a file and check the descriptor
    * Check if the file's initial size is 10 MiB 


### function TEST_F

```cpp
TEST_F(
    BasicFileManagerTest ,
    HandlesPageAllocation 
)
```

Tests page allocation and free. 



1. Allocate 2 pages and free one of them, traverse the free page list and check the existence/absence of the freed/allocated page 


### function TEST_F

```cpp
TEST_F(
    BasicFileManagerTest ,
    CheckReadWriteOperation 
)
```

Tests page read/write operations. 



1. Write/Read a page with some random content and check if the data matches 


### function TEST_F

```cpp
TEST_F(
    BasicFileManagerTest ,
    UniqueIdTest 
)
```

Tests unique table fd. 

Create table files with different path, but same realpath to check if table uses unique id for that file. Also checks real different file and assure that two different table id should not be same. 


### function TEST_F

```cpp
TEST_F(
    BasicFileManagerTest ,
    SequentialAllocateTest 
)
```

Tests sequential allocation. 

Check if pages are correctly allocated and freed. it does not include any expectation, so test procedure purely depends on filemanager's own error checking method. 


### function TEST_F

```cpp
TEST_F(
    BasicFileManagerTest ,
    RandomAllocateTest 
)
```

Tests random allocation. 

This disk space manager uses LIFO for free page management. Allocate and free page randomly first then allocate again and check if second allocation result is equal to first free result in reverse order. 



## Attributes Documentation

### variable test_count

```cpp
constexpr int test_count = 128;
```


### variable TABLE_PATH

```cpp
const char * TABLE_PATH = "test.db";
```


### variable TABLE_PATH_ALIAS

```cpp
const char * TABLE_PATH_ALIAS = "./test.db";
```


### variable ANOTHER_TABLE_PATH

```cpp
const char * ANOTHER_TABLE_PATH = "test_another.db";
```



## Source code

```cpp

#include "file.h"

#include <gtest/gtest.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>

constexpr int test_count = 128;

const char* TABLE_PATH = "test.db";
const char* TABLE_PATH_ALIAS = "./test.db";
const char* ANOTHER_TABLE_PATH = "test_another.db";

class BasicFileManagerTest : public ::testing::Test {
   protected:
    int test_order[test_count];
    tableid_t table_id = 0;

    BasicFileManagerTest() {
        table_id = file_open_table_file(TABLE_PATH);
        srand(time(NULL));

        // Generate random indexes
        for (int i = 0; i < test_count; i++) {
            test_order[i] = i + 1;
        }

        for (int i = 0; i < test_count; i++) {
            int x, y, temp;
            x = rand() % test_count;
            y = rand() % test_count;

            temp = test_order[x];
            test_order[x] = test_order[y];
            test_order[y] = temp;
        }
    }
    ~BasicFileManagerTest() { file_close_table_files(); }
};

TEST_F(BasicFileManagerTest, HandlesInitialization) {
    // Check if the file is opened
    ASSERT_TRUE(table_id >=
                0);  // change the condition to your design's behavior

    headerpage_t header_page;
    file_read_page(table_id, 0, &header_page);
    int num_pages = header_page.page_num;
    EXPECT_EQ(num_pages, INITIAL_TABLE_FILE_SIZE / PAGE_SIZE)
        << "The initial number of pages does not match the requirement: "
        << num_pages;

    struct stat table_stat;
    lstat(TABLE_PATH, &table_stat);
    EXPECT_EQ(table_stat.st_size, 4096 * 2560);
}

TEST_F(BasicFileManagerTest, HandlesPageAllocation) {
    pagenum_t allocated_page, freed_page;

    // Allocate the pages
    allocated_page = file_alloc_page(table_id);
    freed_page = file_alloc_page(table_id);

    // Free one page
    file_free_page(table_id, freed_page);

    // Traverse the free page list and check the existence of the
    // freed/allocated pages. You might need to open a few APIs soley for
    // testing.
    headerpage_t header_page;
    file_read_page(table_id, 0, reinterpret_cast<page_t*>(&header_page));

    // Flag which means if freed page is in free page list correctly.
    bool is_freed_correctly = false;
    int current_page_idx = header_page.free_page_idx;
    freepage_t free_page;

    while (current_page_idx != 0) {
        is_freed_correctly =
            is_freed_correctly || (current_page_idx == freed_page);

        // Expect that an allocated page should not be in free page list.
        EXPECT_NE(current_page_idx, allocated_page);

        file_read_page(table_id, current_page_idx,
                       reinterpret_cast<page_t*>(&free_page));
        current_page_idx = free_page.next_free_idx;
    }
    EXPECT_TRUE(is_freed_correctly);

    file_free_page(table_id, allocated_page);
}

TEST_F(BasicFileManagerTest, CheckReadWriteOperation) {
    int free_page_num = file_alloc_page(table_id);

    ASSERT_EQ(sizeof(internalpage_t), 4096);

    internalpage_t page;
    uint8_t* page_data = reinterpret_cast<uint8_t*>(page.page_branches);

    // Generate random values for write
    uint8_t random_values[PAGE_SIZE - PAGE_HEADER_SIZE] = {};
    for(int i = 0; i < PAGE_SIZE - PAGE_HEADER_SIZE; i++) {
        random_values[i] = rand() % 256;
        // Set that data into allocated page area
        page_data[i] = random_values[i];
    }

    // Write page into file
    file_write_page(table_id, free_page_num, &page);
    // Read page from file
    file_read_page(table_id, free_page_num, &page);

    for(int i = 0; i < PAGE_SIZE - PAGE_HEADER_SIZE; i++) {
        // Validation
        EXPECT_EQ(random_values[i], page_data[i]);
    }

    file_free_page(table_id, free_page_num);
}

TEST_F(BasicFileManagerTest, UniqueIdTest) {
    EXPECT_EQ(table_id, file_open_table_file(TABLE_PATH));
    EXPECT_EQ(table_id, file_open_table_file(TABLE_PATH_ALIAS));

    EXPECT_NE(table_id, file_open_table_file(ANOTHER_TABLE_PATH));
}

TEST_F(BasicFileManagerTest, SequentialAllocateTest) {
    int allocation_result[test_count] = {
        0,
    };

    for (int i = 0; i < test_count; i++) {
        allocation_result[i] = file_alloc_page(table_id);
    }

    for (int i = 0; i < test_count; i++) {
        file_free_page(table_id, allocation_result[i]);
    }
}

TEST_F(BasicFileManagerTest, RandomAllocateTest) {
    for (int i = 0; i < test_count; i++) {
        file_alloc_page(table_id);
    }

    // Randomly free page
    for (int i = 0; i < test_count; i++) {
        file_free_page(table_id, test_order[i]);
    }

    // Check if the allocated page id is equal to freed page in reverse order.
    for (int i = 0; i < test_count; i++) {
        EXPECT_EQ(test_order[test_count - 1 - i], file_alloc_page(table_id));
    }

    for (int i = 0; i < test_count; i++) {
        file_free_page(table_id, test_order[i]);
    }
}
```


-------------------------------

Updated on 2021-10-16 at 21:43:54 +0900

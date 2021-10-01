

# TestCode



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
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , UniqueIdTest )<br>Tests unique database fd.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , SequentialAllocateTest )<br>Tests sequential allocation.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , RandomAllocateTest )<br>Tests random allocation.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[test_count](/Modules/TestCode#variable-test_count)**  |
| const char * | **[DATABASE_PATH](/Modules/TestCode#variable-database_path)**  |
| const char * | **[DATABASE_PATH_ALIAS](/Modules/TestCode#variable-database_path_alias)**  |
| const char * | **[ANOTHER_DATABASE_PATH](/Modules/TestCode#variable-another_database_path)**  |


## Functions Documentation

### function TEST_F

```
TEST_F(
    BasicFileManagerTest ,
    HandlesInitialization 
)
```

Tests file open/close APIs. 



1. Open a file and check the descriptor
    * Check if the file's initial size is 10 MiB 


### function TEST_F

```
TEST_F(
    BasicFileManagerTest ,
    HandlesPageAllocation 
)
```

Tests page allocation and free. 



1. Allocate 2 pages and free one of them, traverse the free page list and check the existence/absence of the freed/allocated page 


### function TEST_F

```
TEST_F(
    BasicFileManagerTest ,
    CheckReadWriteOperation 
)
```

Tests page read/write operations. 



1. Write/Read a page with some random content and check if the data matches 


### function TEST_F

```
TEST_F(
    BasicFileManagerTest ,
    UniqueIdTest 
)
```

Tests unique database fd. 

Create database files with different path, but same realpath to check if database uses unique id for that file. Also checks real different file and assure that two different database fd should not be same. 


### function TEST_F

```
TEST_F(
    BasicFileManagerTest ,
    SequentialAllocateTest 
)
```

Tests sequential allocation. 

Check if pages are correctly allocated and freed. it does not include any expectation, so test procedure purely depends on filemanager's own error checking method. 


### function TEST_F

```
TEST_F(
    BasicFileManagerTest ,
    RandomAllocateTest 
)
```

Tests random allocation. 

This disk space manager uses LIFO for free page management. Allocate and free page randomly first then allocate again and check if second allocation result is equal to first free result in reverse order. 



## Attributes Documentation

### variable test_count

```
constexpr int test_count = 128;
```


### variable DATABASE_PATH

```
const char * DATABASE_PATH = "test.db";
```


### variable DATABASE_PATH_ALIAS

```
const char * DATABASE_PATH_ALIAS = "./test.db";
```


### variable ANOTHER_DATABASE_PATH

```
const char * ANOTHER_DATABASE_PATH = "test_another.db";
```





-------------------------------

Updated on 2021-10-01 at 23:30:07 +0900
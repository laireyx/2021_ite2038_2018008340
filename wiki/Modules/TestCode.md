

# TestCode



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[BasicFileManagerTest](/Classes/BasicFileManagerTest)**  |
| class | **[BasicTableTest](/Classes/BasicTableTest)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , HandlesInitialization )<br>Tests file open/close APIs.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , HandlesPageAllocation )<br>Tests page allocation and free.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , CheckReadWriteOperation )<br>Tests page read/write operations.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , UniqueIdTest )<br>Tests unique table fd.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , SequentialAllocateTest )<br>Tests sequential allocation.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicFileManagerTest">BasicFileManagerTest</a> , RandomAllocateTest )<br>Tests random allocation.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicTableTest">BasicTableTest</a> , RandomDeletionTest )<br>Tests database deletion API.  |
| | **[TEST_F](/Modules/TestCode#function-test_f)**(<a href="/Classes/BasicTableTest">BasicTableTest</a> , RandomInsertTest )<br>Tests database insertion API.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[test_count](/Modules/TestCode#variable-test_count)**  |
| const char * | **[TABLE_PATH](/Modules/TestCode#variable-table_path)**  |
| const char * | **[TABLE_PATH_ALIAS](/Modules/TestCode#variable-table_path_alias)**  |
| const char * | **[ANOTHER_TABLE_PATH](/Modules/TestCode#variable-another_table_path)**  |
| constexpr int | **[test_count](/Modules/TestCode#variable-test_count)**  |


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

Tests unique table fd. 

Create table files with different path, but same realpath to check if table uses unique id for that file. Also checks real different file and assure that two different table id should not be same. 


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


### function TEST_F

```
TEST_F(
    BasicTableTest ,
    RandomDeletionTest 
)
```

Tests database deletion API. 



1. Open a database and write random values in random order.
    * Removes those values in random order.
    * Find those values and check existency. 


### function TEST_F

```
TEST_F(
    BasicTableTest ,
    RandomInsertTest 
)
```

Tests database insertion API. 



1. Open a database and write random values in random order.
    * Find the value using the key and compare it to the value. 



## Attributes Documentation

### variable test_count

```
constexpr int test_count = 128;
```


### variable TABLE_PATH

```
const char * TABLE_PATH = "test.db";
```


### variable TABLE_PATH_ALIAS

```
const char * TABLE_PATH_ALIAS = "./test.db";
```


### variable ANOTHER_TABLE_PATH

```
const char * ANOTHER_TABLE_PATH = "test_another.db";
```


### variable test_count

```
constexpr int test_count = 128;
```





-------------------------------

Updated on 2021-10-16 at 00:40:34 +0900
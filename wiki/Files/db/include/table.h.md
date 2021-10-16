

# db/include/table.h



## Namespaces

| Name           |
| -------------- |
| **[table_helper](/Namespaces/table_helper)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[init_db](/Modules/TableManager#function-init_db)**()<br>Initialize database management system.  |
| tableid_t | **[open_table](/Modules/TableManager#function-open_table)**(char * pathname)<br>Open existing data file using ‘pathname’ or create one if not existed.  |
| int | **[db_insert](/Modules/TableManager#function-db_insert)**(tableid_t table_id, int64_t key, char * value, uint16_t value_size)<br>Insert input (key, value) record with its size to data file at the right place.  |
| int | **[db_find](/Modules/TableManager#function-db_find)**(tableid_t table_id, int64_t key, char * ret_val, uint16_t * value_size)<br>Find the record corresponding the input key.  |
| int | **[db_delete](/Modules/TableManager#function-db_delete)**(tableid_t table_id, int64_t key)<br>Find the matching record and delete it if found.  |
| int | **[shutdown_db](/Modules/TableManager#function-shutdown_db)**()<br>Shutdown database management system.  |


## Functions Documentation

### function init_db

```cpp
int init_db()
```

Initialize database management system. 

**Return**: If success, return 0. Otherwise return non-zero value. 

### function open_table

```cpp
tableid_t open_table(
    char * pathname
)
```

Open existing data file using ‘pathname’ or create one if not existed. 

**Parameters**: 

  * **pathname** Table file path. 


**Return**: unique table id which represents the own table in this database. return negative value otherwise. 

### function db_insert

```cpp
int db_insert(
    tableid_t table_id,
    int64_t key,
    char * value,
    uint16_t value_size
)
```

Insert input (key, value) record with its size to data file at the right place. 

**Parameters**: 

  * **table_id** Table id obtained with <code><a href="/Modules/TableManager#function-open-table">open&#95;table()</a></code>. 
  * **key** Record key. 
  * **value** Record value. 
  * **value_size** Record value size. 


**Return**: 0 if success. negative value otherwise. 

### function db_find

```cpp
int db_find(
    tableid_t table_id,
    int64_t key,
    char * ret_val,
    uint16_t * value_size
)
```

Find the record corresponding the input key. 

**Parameters**: 

  * **table_id** Table id obtained with <code><a href="/Modules/TableManager#function-open-table">open&#95;table()</a></code>. 
  * **key** Record key. 
  * **value** Record value. 
  * **value_size** Record value size. 


**Return**: 0 if success. negative value otherwise. 

If found, ret_val and value_size will be set to matching value and its size.


### function db_delete

```cpp
int db_delete(
    tableid_t table_id,
    int64_t key
)
```

Find the matching record and delete it if found. 

**Parameters**: 

  * **table_id** Table id obtained with <code><a href="/Modules/TableManager#function-open-table">open&#95;table()</a></code>. 
  * **key** Record key. 


**Return**: 0 if success. negative value otherwise. 

### function shutdown_db

```cpp
int shutdown_db()
```

Shutdown database management system. 

**Return**: If success, return 0. Otherwise return non-zero value. 



## Source code

```cpp

#pragma once

#include "types.h"

namespace table_helper {
bool switch_to_id(int64_t table_id);
}  // namespace table_helper

int init_db();

tableid_t open_table(char* pathname);

int db_insert(tableid_t table_id, int64_t key, char* value,
              uint16_t value_size);

int db_find(tableid_t table_id, int64_t key, char* ret_val,
            uint16_t* value_size);

int db_delete(tableid_t table_id, int64_t key);

int shutdown_db();
```


-------------------------------

Updated on 2021-10-16 at 20:56:45 +0900

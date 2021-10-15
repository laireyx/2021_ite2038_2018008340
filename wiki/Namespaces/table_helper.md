

# table_helper

**Module:** **[TableManager](/Modules/TableManager)**



## Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[switch_to_id](/Namespaces/table_helper#function-switch_to_id)**(int64_t table_id)<br>Switch current table into given table id.  |


## Functions Documentation

### function switch_to_id

```cpp
bool switch_to_id(
    int64_t table_id
)
```

Switch current table into given table id. 

**Parameters**: 

  * **table_id** table id obtained with <code><a href="/Modules/DiskSpaceManager#function-file-open-table-file">file&#95;open&#95;table&#95;file()</a></code>. 


If current table_fd is already means given table id, then do nothing. If not, change table_fd to given fd and re-read header_page from it.






-------------------------------

Updated on 2021-10-16 at 00:40:34 +0900
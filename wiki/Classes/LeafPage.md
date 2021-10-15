

# LeafPage

**Module:** **[DiskSpaceManager](/Modules/DiskSpaceManager)**



struct for allocated leaf page. 


`#include <page.h>`

Inherits from [AllocatedPage](/Classes/AllocatedPage), [Page](/Classes/Page)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint8_t | **[reserved](/Classes/LeafPage#variable-reserved)** <br>Reserved area for normal allocated page.  |

## Additional inherited members

**Public Attributes inherited from [AllocatedPage](/Classes/AllocatedPage)**

|                | Name           |
| -------------- | -------------- |
| <a href="/Classes/PageHeader">PageHeader</a> | **[page_header](/Classes/AllocatedPage#variable-page_header)** <br><a href="/Classes/Page">Page</a> header.  |


## Public Attributes Documentation

### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for normal allocated page. 

-------------------------------

Updated on 2021-10-16 at 00:31:48 +0900
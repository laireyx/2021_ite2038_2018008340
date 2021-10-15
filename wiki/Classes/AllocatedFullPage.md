

# AllocatedFullPage

**Module:** **[DiskSpaceManager](/Modules/based B+ treeSpaceManager)**



struct for any allocated page.  [More...](#detailed-description)


`#include <page.h>`

Inherits from [AllocatedPage](/Classes/AllocatedPage), [Page](/Classes/Page)

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint8_t | **[reserved](/Classes/AllocatedFullPage#variable-reserved)** <br>Reserved area for normal allocated page.  |

## Additional inherited members

**Public Attributes inherited from [AllocatedPage](/Classes/AllocatedPage)**

|                | Name           |
| -------------- | -------------- |
| <a href="/Classes/PageHeader">PageHeader</a> | **[page_header](/Classes/AllocatedPage#variable-page_header)** <br><a href="/Classes/Page">Page</a> header.  |


## Detailed Description

```cpp
class AllocatedFullPage;
```

struct for any allocated page. 

You should use this struct for read & write allocated page. 

## Public Attributes Documentation

### variable reserved

```cpp
uint8_t reserved;
```

Reserved area for normal allocated page. 

-------------------------------

Updated on 2021-10-15 at 13:42:29 +0900
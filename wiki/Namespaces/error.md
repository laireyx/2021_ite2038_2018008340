

# error

Error.  [More...](#detailed-description)

## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[print](/Namespaces/error#function-print)**(bool exit_flag =false)<br>Print error message of errno and exit if flag is set.  |
| bool | **[check](/Namespaces/error#function-check)**(int value, bool exit_flag =false)<br>If value < 0, then print error message.  |

## Detailed Description

Error. 

This namespace includes some useful functions which are used to print, handle an error(or even abort and exit). 


## Functions Documentation

### function print

```cpp
inline int print(
    bool exit_flag =false
)
```

Print error message of errno and exit if flag is set. 

**Parameters**: 

  * **exit_flag** if set to true, then print error message and terminate program with error code 1. 


**Return**: -errno for return _perrno(); in some functions. 

If exit_flag = false, just print errno message. If exit_flag = true, print errno message and exit.


### function check

```cpp
inline bool check(
    int value,
    bool exit_flag =false
)
```

If value < 0, then print error message. 

**Parameters**: 

  * **exit_flag** if set to true, then print error message and terminate program with error code 1. 


**Return**: true if check is success, and false if fails(for just printing and not terminating). 





-------------------------------

Updated on 2021-09-29 at 00:56:15 +0900


# error

Error.  [More...](#detailed-description)

## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[print](/Namespaces/error#function-print)**(bool exit_flag =false)<br>Print error message of errno and exit if flag is set.  |
| bool | **[check](/Namespaces/error#function-check)**(int value, bool exit_flag =false)<br>If <code>value &lt; 0</code>, then print error message.  |

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


**Return**: -errno for <code>return <a href="/Namespaces/error#function-print">error::print()</a>;</code> in some functions. 

If <code>exit&#95;flag == true</code>, print errno message and exit. If <code>exit&#95;flag == false</code>, just print errno message.


### function check

```cpp
inline bool check(
    int value,
    bool exit_flag =false
)
```

If <code>value &lt; 0</code>, then print error message. 

**Parameters**: 

  * **exit_flag** if set to true, then print error message and terminate program with error code 1. 


**Return**: true if check is success, and false if fails(for just printing and not terminating). 





-------------------------------

Updated on 2021-09-29 at 01:17:26 +0900
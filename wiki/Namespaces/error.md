

# error

Error.  [More...](#detailed-description)

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[error::Exception](/Classes/error::Exception)**  |

## Functions

|                | Name           |
| -------------- | -------------- |
| int | **[print](/Namespaces/error#function-print)**(bool exit_flag =<a href="/Namespaces/error#variable-print-error">PRINT_ERROR</a>)<br>Print error message of errno and exit if flag is set.  |
| bool | **[ok](/Namespaces/error#function-ok)**(bool assertion, bool exit_flag =<a href="/Namespaces/error#variable-throw-exception">THROW_EXCEPTION</a>)<br>If <code>assertion = false</code>, then throw error.  |

## Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[SILENT](/Namespaces/error#variable-silent)** <br>If set, ignore all other flags and just return quietly.  |
| constexpr int | **[PRINT_ERROR](/Namespaces/error#variable-print_error)** <br>If set, print error.  |
| constexpr int | **[THROW_EXCEPTION](/Namespaces/error#variable-throw_exception)** <br>If set, throw exception.  |
| constexpr int | **[EXIT_PROGRAM](/Namespaces/error#variable-exit_program)** <br>If set, immediately exit program with exit(1);.  |

## Detailed Description

Error. 

This namespace includes some useful functions which are used to print, handle an error(or even abort and exit). 


## Functions Documentation

### function print

```cpp
inline int print(
    bool exit_flag =PRINT_ERROR
)
```

Print error message of errno and exit if flag is set. 

**Parameters**: 

  * **exit_flag** if set to true, then print error message and terminate program with error code 1. 


**Return**: -errno for <code>return <a href="/Namespaces/error#function-print">error::print()</a>;</code> in some functions. 

If <code>exit&#95;flag == true</code>, print errno message and exit. If <code>exit&#95;flag == false</code>, just print errno message.


### function ok

```cpp
inline bool ok(
    bool assertion,
    bool exit_flag =THROW_EXCEPTION
)
```

If <code>assertion = false</code>, then throw error. 

**Parameters**: 

  * **assertion** if <code>assertion = true</code>, then return silently. if <code>assertion = false</code>, then throw <a href="/Classes/error::Exception">Exception</a>. 



## Attributes Documentation

### variable SILENT

```cpp
constexpr int SILENT = 0x0001;
```

If set, ignore all other flags and just return quietly. 

### variable PRINT_ERROR

```cpp
constexpr int PRINT_ERROR = 0x0002;
```

If set, print error. 

### variable THROW_EXCEPTION

```cpp
constexpr int THROW_EXCEPTION = 0x0004;
```

If set, throw exception. 

### variable EXIT_PROGRAM

```cpp
constexpr int EXIT_PROGRAM = 0x0008;
```

If set, immediately exit program with exit(1);. 




-------------------------------

Updated on 2021-10-01 at 23:30:07 +0900
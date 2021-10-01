#pragma once

#include <errno.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

/**
 * @brief   Error
 * @details This namespace includes some useful functions which are used to
 *          print, handle an error(or even abort and exit).
 */
namespace error {

/// @brief If set, ignore all other flags and just return quietly.
constexpr int SILENT            = 0x0001;
/// @brief If set, print error.
constexpr int PRINT_ERROR       = 0x0002;
/// @brief If set, throw exception.
constexpr int THROW_EXCEPTION   = 0x0004;
/// @brief If set, immediately exit program with exit(1);
constexpr int EXIT_PROGRAM      = 0x0008;

class Exception {
private:
    /// @brief exception message
    std::string error_message;
    
    /// @brief Make Exception noncopyable
    Exception(const Exception& rhs);
    /// @brief Make Exception noncopyable
    Exception& operator=(const Exception&);
public:
    Exception(std::string message) : error_message(message) { }
    ~Exception() { }
};

/**
 * @brief    Print error message of errno and exit if flag is set.
 * @details  If <code>exit_flag == true</code>, print errno message and exit.
 *           If <code>exit_flag == false</code>, just print errno message.
 *
 * @param    exit_flag  if set to true, then print error message and terminate
 *                      program with error code 1.
 * @returns  -errno     for <code>return error::print();</code> in some
 * functions.
 */
inline int print(bool exit_flag = PRINT_ERROR) {
    if(exit_flag & SILENT) {
        return -errno;
    }

    if(exit_flag & PRINT_ERROR) {
        perror("");
    }
    if(exit_flag & THROW_EXCEPTION) {
        throw new Exception("Error");
    }
    if(exit_flag & EXIT_PROGRAM) {
        exit(1);
    }

    return -errno;
}

/**
 * @brief    If <code>assertion = false</code>, then throw error.
 *
 * @param    assertion   if <code>assertion = true</code>, then return silently.
 *                       if <code>assertion = false</code>, then throw Exception.
 */
inline bool ok(bool assertion, bool exit_flag = THROW_EXCEPTION) {
    if (!assertion) {
        if(exit_flag & SILENT) {
            return false;
        }
        if(exit_flag & PRINT_ERROR) {
            print(exit_flag);
        }
        if(exit_flag & THROW_EXCEPTION) {
            throw new Exception("Error");
        }
        if(exit_flag & EXIT_PROGRAM) {
            exit(1);
        }
        return false;
    }
    return true;
}

}  // namespace error
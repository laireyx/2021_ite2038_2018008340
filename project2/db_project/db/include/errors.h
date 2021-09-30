#pragma once

#include <errno.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
 * @brief   Error
 * @details This namespace includes some useful functions which are used to
 *          print, handle an error(or even abort and exit).
 */
namespace error {

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
inline int print(bool exit_flag = false) {
    perror(strerror(errno));

    if (exit_flag) {
        exit(1);
    }

    return -errno;
}

/**
 * @brief    If <code>value < 0</code>, then print error message.
 *
 * @param    exit_flag   if set to true, then print error message and terminate
 *                       program with error code 1.
 * @returns              true if check is success, and false if fails(for just
 *                       printing and not terminating).
 */
inline bool check(int value, bool exit_flag = false) {
    if (value < 0) {
        print(exit_flag);
        return false;
    }
    return true;
}

}  // namespace error
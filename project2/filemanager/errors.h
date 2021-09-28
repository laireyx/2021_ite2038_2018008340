#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

/**
 * @brief   Print error message of errno and exit if flag is set.
 * @details If exit_flag = false, just print errno message.
 *          If exit_flag = true, print errno message and exit.
 *
 * @returns -errno for return _perrno(); in some functions.
 */
inline int _perrno(bool exit_flag = false) {
    perror(strerror(errno));
    
    if(exit_flag) {
        exit(1);
    }

    return -errno;
}

/**
 * @def     CHECK(value)
 * @brief   If value < 0, then print error message.
 */
#define CHECK(value) do { \
        if((value) < 0) { \
            _perrno(); \
        } \
    } while(0)

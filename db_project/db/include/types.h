#pragma once

#include <cstdint>

typedef uint64_t pagenum_t;

#include "page.h"

typedef Page page_t;
typedef AllocatedPage allocatedpage_t;
typedef HeaderPage headerpage_t;
typedef FreePage freepage_t;

/**
 * @class   DatabaseInstance
 * @brief   Database file instance.
 */
typedef struct DatabaseInstance {
    /// @brief Real database file path(obtained by realpath(3)).
    char* file_path;
    /// @brief Database file descriptor.
    int file_descriptor;
} DatabaseInstance;
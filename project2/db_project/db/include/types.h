#pragma once

#include <cstdint>

typedef uint64_t pagenum_t;

#include "page.h"

typedef Page page_t;
typedef PageHeader pageheader_t;
typedef HeaderPage headerpage_t;
typedef FreePage freepage_t;
typedef AllocatedPage allocatedpage_t;
typedef InternalPage internalpage_t;
typedef LeafPage leafpage_t;

/**
 * @class   TableInstance
 * @brief   Table file instance.
 */
typedef struct TableInstance {
    /// @brief Real table file path(obtained by realpath(3)).
    char* file_path;
    /// @brief Table file descriptor.
    int file_descriptor;
} TableInstance;
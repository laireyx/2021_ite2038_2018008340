#pragma once

#include <cstdint>
#include <memory>

typedef uint64_t pagenum_t;
typedef uint64_t tableid_t;

typedef std::shared_ptr<char[]> value_t;
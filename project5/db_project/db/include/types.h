#pragma once

#include <cstdint>
#include <functional>
#include <memory>

typedef uint64_t pagenum_t;
typedef int64_t tableid_t;

typedef int64_t recordkey_t;

typedef std::pair<tableid_t, pagenum_t> PageLocation;
typedef std::pair<tableid_t, int64_t> LockLocation;
#pragma once

#include <cstdint>
#include <functional>
#include <memory>

typedef int trxid_t;
typedef uint64_t trxlogid_t;

typedef uint64_t pagenum_t;
typedef int64_t tableid_t;

typedef int64_t recordkey_t;
typedef uint16_t valsize_t;

typedef uint64_t lockmask_t;

typedef std::pair<tableid_t, pagenum_t> PageLocation, LockLocation;

namespace std {
template <>
struct hash<PageLocation> {
    size_t operator()(const PageLocation& location) const {
        size_t hash_value = 17;
        hash_value = hash_value * 31 + std::hash<tableid_t>()(location.first);
        hash_value = hash_value * 31 + std::hash<pagenum_t>()(location.second);
        return hash_value;
    }
};
}  // namespace std
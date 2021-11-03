/**
 * @addtogroup LockManager
 * @{
 */
#pragma once

#include <cstdint>

#include <functional>

typedef std::pair<int, int64_t> LockLocation;

struct lock_t {
  /* GOOD LOCK :) */
  int table_id;
  int64_t key;

  bool operator==(const lock_t& rhs) const {
    return (table_id == rhs.table_id) && (key == rhs.key);
  }
};

namespace std {
template <>
struct hash<lock_t> {
    size_t operator()(const lock_t& lock) const {
        size_t hash_value = 17;
        hash_value = hash_value * 31 + std::hash<int>()(lock.table_id);
        hash_value = hash_value * 31 + std::hash<int64_t>()(lock.key);
        return hash_value;
    }
};
}  // namespace std

typedef struct lock_t lock_t;

/* APIs for lock table */
int init_lock_table();
lock_t *lock_acquire(int table_id, int64_t key);
int lock_release(lock_t* lock_obj);
/** @}*/
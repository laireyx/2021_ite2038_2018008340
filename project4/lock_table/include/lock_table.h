/**
 * @addtogroup LockManager
 * @{
 */
#pragma once

#include <pthread.h>

#include <cstdint>
#include <functional>

typedef std::pair<int, int64_t> LockLocation;

struct lock_t {
  /* GOOD LOCK :) */
  LockLocation lock_location;

  pthread_cond_t* cond;

  lock_t* prev;
  lock_t* next;
};

namespace std {
template <>
struct hash<LockLocation> {
    size_t operator()(const LockLocation& location) const {
        size_t hash_value = 17;
        hash_value = hash_value * 31 + std::hash<int>()(location.first);
        hash_value = hash_value * 31 + std::hash<int64_t>()(location.second);
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
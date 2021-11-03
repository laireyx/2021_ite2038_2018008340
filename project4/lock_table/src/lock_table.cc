/**
 * @addtogroup LockManager
 * @{
 */
#include "lock_table.h"

#include <pthread.h>
#include <iostream>
#include <unordered_set>

pthread_cond_t *table_condition;
pthread_mutex_t *table_condition_mutex;

pthread_cond_t lock_condition;
pthread_mutex_t lock_mutex;

std::unordered_set<lock_t> lock_instances;

int log_cnt = 10000;

int init_lock_table() {

  pthread_cond_init(&lock_condition, nullptr);
  pthread_mutex_init(&lock_mutex, nullptr);

  /** @todo change it to MAX_TABLE_INSTANCE */
  table_condition = new pthread_cond_t[32];
  table_condition_mutex = new pthread_mutex_t[32];

  for(int i = 0; i < 32; i++) {
    pthread_cond_init(&table_condition[i], nullptr);
    pthread_mutex_init(&table_condition_mutex[i], nullptr);
  }
  return 0;
}

lock_t* lock_acquire(int table_id, int64_t key) {
  lock_t* lock_instance = new lock_t;
  bool can_acquire_immediately = false;

  lock_instance->table_id = table_id;
  lock_instance->key = key;

  //std::cout << log_cnt++ << " " << "ACQ start lock" << std::endl;
  //pthread_mutex_lock(&table_condition_mutex[table_id]);
  pthread_mutex_lock(&lock_mutex);
  while(true) {
    if(!lock_instances.count(*lock_instance)) {
      lock_instances.emplace(*lock_instance);

      //std::cout << log_cnt++ << " " << "ACQ success unlock" << std::endl;
      //pthread_mutex_unlock(&table_condition_mutex[table_id]);
      pthread_mutex_unlock(&lock_mutex);
      return lock_instance;
    }
    //std::cout << log_cnt++ << " " << "ACQ failed wait" << std::endl;
    //pthread_cond_wait(&table_condition[table_id], &table_condition_mutex[table_id]);
    pthread_cond_wait(&lock_condition, &lock_mutex);
  }
};

int lock_release(lock_t* lock_obj) {
  int table_id;
  int64_t key;

  table_id = lock_obj->table_id;
  key = lock_obj->key;

  //std::cout << log_cnt++ << " " << "REL start lock" << std::endl;
  //pthread_mutex_lock(&table_condition_mutex[table_id]);
  pthread_mutex_lock(&lock_mutex);
  const auto& existing_lock = lock_instances.find(*lock_obj);
  if(existing_lock != lock_instances.end()) {
    lock_instances.erase(existing_lock);
    delete lock_obj;
  
    //std::cout << log_cnt++ << " " << "REL success unlock" << std::endl;
    //pthread_cond_broadcast(&table_condition[table_id]);
    pthread_cond_broadcast(&lock_condition);
    //pthread_mutex_unlock(&table_condition_mutex[table_id]);
    pthread_mutex_unlock(&lock_mutex);
    return 0;
  } else {
    //std::cout << log_cnt++ << " " << "REL fail unlock" << std::endl;
    //pthread_cond_broadcast(&table_condition[table_id]);
    pthread_cond_broadcast(&lock_condition);
    //pthread_mutex_unlock(&table_condition_mutex[table_id]);
    pthread_mutex_unlock(&lock_mutex);
    return -1;
  }
}
/** @}*/
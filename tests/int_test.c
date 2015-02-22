#include <assert.h>
#include <stdlib.h>

#include "hash_table.h"


static bool
equal_int_keys(void const *first_key, void const *second_key);

static uint32_t
hash_int_key(void const *key);


int
int_test(void)
{
  struct hash_table *hash_table = hash_table_alloc(10, hash_int_key, equal_int_keys);
  assert(hash_table);
  assert(10 == hash_table_capacity(hash_table));
  assert(0 == hash_table_count(hash_table));
  
  void const **keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  free(keys);
  
  int key1 = 1;
  int value1 = 10;
  assert( ! hash_table_has_key(hash_table, &key1));
  
  void const *previous_value;
  int result = hash_table_put(hash_table, &key1, &value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, &key1));
  assert(&value1 == hash_table_get(hash_table, &key1));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0]);
  free(keys);
  
  int key2 = 2;
  int value2 = 20;
  assert( ! hash_table_has_key(hash_table, &key2));
  
  result = hash_table_put(hash_table, &key2, &value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, &key2));
  assert(&value2 == hash_table_get(hash_table, &key2));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1]);
  assert(&key2 == keys[0] || &key2 == keys[1]);
  free(keys);
  
  int key3 = 3;
  int value3a = 30;
  assert( ! hash_table_has_key(hash_table, &key3));
  
  result = hash_table_put(hash_table, &key3, &value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, &key3));
  assert(&value3a == hash_table_get(hash_table, &key3));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1] || &key1 == keys[2]);
  assert(&key2 == keys[0] || &key2 == keys[1] || &key2 == keys[2]);
  assert(&key3 == keys[0] || &key3 == keys[1] || &key3 == keys[2]);
  free(keys);
  
  int value3b = 33;
  result = hash_table_put(hash_table, &key3, &value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(&value3a == previous_value);
  assert(hash_table_has_key(hash_table, &key3));
  assert(&value3b == hash_table_get(hash_table, &key3));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1] || &key1 == keys[2]);
  assert(&key2 == keys[0] || &key2 == keys[1] || &key2 == keys[2]);
  assert(&key3 == keys[0] || &key3 == keys[1] || &key3 == keys[2]);
  free(keys);
  
  int key4 = 4;
  void const *removed_value;
  hash_table_remove(hash_table, &key4, &removed_value);
  assert(NULL == removed_value);
  assert(3 == hash_table_count(hash_table));
  
  hash_table_remove(hash_table, &key3, &removed_value);
  assert(&value3b == removed_value);
  assert(2 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1]);
  assert(&key2 == keys[0] || &key2 == keys[1]);
  free(keys);
  
  hash_table_remove(hash_table, &key1, &removed_value);
  assert(&value1 == removed_value);
  assert(1 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key2 == keys[0]);
  free(keys);
  
  hash_table_remove(hash_table, &key2, &removed_value);
  assert(&value2 == removed_value);
  assert(0 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  free(keys);
  
  hash_table_free(hash_table);
  return 0;
}


static bool
equal_int_keys(void const *first_key, void const *second_key)
{
  int const *first_int_key = first_key;
  int const *second_int_key = second_key;
  return *first_int_key == *second_int_key;
}


static uint32_t
hash_int_key(void const *key)
{
  int const *int_key = key;
  return abs(*int_key) % UINT32_MAX;
}

#include <assert.h>
#include <stdlib.h>

#include "hash_table.h"


static bool
equal_int_keys(void const *first_key, void const *second_key);

static size_t
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
  union ht_value value1 = {.int_value=10,};
  assert( ! hash_table_has_key(hash_table, &key1));
  
  union ht_value previous_value;
  int result = hash_table_put(hash_table, &key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  assert(hash_table_has_key(hash_table, &key1));
  union ht_value value;
  assert(0 == hash_table_get(hash_table, &key1, &value));
  assert(value1.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0]);
  free(keys);
  
  int key2 = 2;
  union ht_value value2 = {.int_value=20,};
  assert( ! hash_table_has_key(hash_table, &key2));
  
  result = hash_table_put(hash_table, &key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(hash_table_has_key(hash_table, &key2));
  assert(0 == hash_table_get(hash_table, &key2, &value));
  assert(value2.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1]);
  assert(&key2 == keys[0] || &key2 == keys[1]);
  free(keys);
  
  int key3 = 3;
  union ht_value value3a = {.int_value=30,};
  assert( ! hash_table_has_key(hash_table, &key3));
  
  result = hash_table_put(hash_table, &key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(hash_table_has_key(hash_table, &key3));
  assert(0 == hash_table_get(hash_table, &key3, &value));
  assert(value3a.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1] || &key1 == keys[2]);
  assert(&key2 == keys[0] || &key2 == keys[1] || &key2 == keys[2]);
  assert(&key3 == keys[0] || &key3 == keys[1] || &key3 == keys[2]);
  free(keys);
  
  union ht_value value3b = {.int_value=33,};
  result = hash_table_put(hash_table, &key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(value3a.int_value == previous_value.int_value);
  assert(hash_table_has_key(hash_table, &key3));
  assert(0 == hash_table_get(hash_table, &key3, &value));
  assert(value3b.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1] || &key1 == keys[2]);
  assert(&key2 == keys[0] || &key2 == keys[1] || &key2 == keys[2]);
  assert(&key3 == keys[0] || &key3 == keys[1] || &key3 == keys[2]);
  free(keys);
  
  int key4 = 4;
  union ht_value removed_value;
  result = hash_table_remove(hash_table, &key4, &removed_value);
  assert(-1 == result);
  assert(3 == hash_table_count(hash_table));
  
  result = hash_table_remove(hash_table, &key3, &removed_value);
  assert(0 == result);
  assert(value3b.int_value == removed_value.int_value);
  assert(2 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key1 == keys[0] || &key1 == keys[1]);
  assert(&key2 == keys[0] || &key2 == keys[1]);
  free(keys);
  
  result = hash_table_remove(hash_table, &key1, &removed_value);
  assert(0 == result);
  assert(value1.int_value == removed_value.int_value);
  assert(1 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(&key2 == keys[0]);
  free(keys);
  
  result = hash_table_remove(hash_table, &key2, &removed_value);
  assert(0 == result);
  assert(value2.int_value == removed_value.int_value);
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


static size_t
hash_int_key(void const *key)
{
  int const *int_key = key;
  return abs(*int_key) % SIZE_MAX;
}

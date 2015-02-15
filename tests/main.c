#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"


static int
compare_int_keys(void *first_key, void *second_key);

static uint32_t
hash_int_key(void *key);


int
main(int argc, char const *argv[])
{
  struct hash_table *hash_table = hash_table_alloc(10, hash_int_key, compare_int_keys);
  assert(hash_table);
  assert(10 == hash_table_capacity(hash_table));
  assert(0 == hash_table_count(hash_table));
  
  int key1 = 1;
  int value1 = 10;
  assert( ! hash_table_has_key(hash_table, &key1));
  
  void *previous_value;
  int result = hash_table_put(hash_table, &key1, &value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, &key1));
  assert(&value1 == hash_table_get(hash_table, &key1));
  
  int key2 = 2;
  int value2 = 20;
  assert( ! hash_table_has_key(hash_table, &key2));
  
  result = hash_table_put(hash_table, &key2, &value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, &key2));
  assert(&value2 == hash_table_get(hash_table, &key2));
  
  int key3 = 3;
  int value3a = 30;
  assert( ! hash_table_has_key(hash_table, &key3));
  
  result = hash_table_put(hash_table, &key3, &value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, &key3));
  assert(&value3a == hash_table_get(hash_table, &key3));
  
  int value3b = 33;
  result = hash_table_put(hash_table, &key3, &value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(&value3a == previous_value);
  assert(hash_table_has_key(hash_table, &key3));
  assert(&value3b == hash_table_get(hash_table, &key3));
  
  int key4 = 4;
  void *removed_value = hash_table_remove(hash_table, &key4);
  assert(NULL == removed_value);
  assert(3 == hash_table_count(hash_table));
  
  removed_value = hash_table_remove(hash_table, &key3);
  assert(&value3b == removed_value);
  assert(2 == hash_table_count(hash_table));
  
  removed_value = hash_table_remove(hash_table, &key1);
  assert(&value1 == removed_value);
  assert(1 == hash_table_count(hash_table));
  
  removed_value = hash_table_remove(hash_table, &key2);
  assert(&value2 == removed_value);
  assert(0 == hash_table_count(hash_table));
  
  hash_table_free(hash_table);
  return 0;
}


static int
compare_int_keys(void *first_key, void *second_key)
{
  int *first_int_key = first_key;
  int *second_int_key = second_key;
  if (*first_int_key < *second_int_key) return -1;
  if (*first_int_key > *second_int_key) return 1;
  return 0;
}


static uint32_t
hash_int_key(void *key)
{
  int *int_key = key;
  return abs(*int_key) % UINT32_MAX;
}

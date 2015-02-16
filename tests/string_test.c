#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"


static bool
equal_string_keys(void const *first_key, void const *second_key);

static uint32_t
hash_string_key(void const *key);


int
string_test(void)
{
  struct hash_table *hash_table = hash_table_alloc(10, hash_string_key, equal_string_keys);
  assert(hash_table);
  assert(10 == hash_table_capacity(hash_table));
  assert(0 == hash_table_count(hash_table));
  
  void const **keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  free(keys);
  
  char const key1[] = "red";
  char const value1[] = "10";
  assert( ! hash_table_has_key(hash_table, key1));
  
  void const *previous_value;
  int result = hash_table_put(hash_table, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, key1));
  assert(value1 == hash_table_get(hash_table, key1));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0]);
  free(keys);
  
  char const key2[] = "green";
  char const value2[] = "20";
  assert( ! hash_table_has_key(hash_table, key2));
  
  result = hash_table_put(hash_table, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, key2));
  assert(value2 == hash_table_get(hash_table, key2));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1]);
  assert(key2 == keys[0] || key2 == keys[1]);
  free(keys);
  
  char const key3[] = "blue";
  char const value3a[] = "30";
  assert( ! hash_table_has_key(hash_table, key3));
  
  result = hash_table_put(hash_table, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, key3));
  assert(value3a == hash_table_get(hash_table, key3));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1] || key1 == keys[2]);
  assert(key2 == keys[0] || key2 == keys[1] || key2 == keys[2]);
  assert(key3 == keys[0] || key3 == keys[1] || key3 == keys[2]);
  free(keys);
  
  char const value3b[] = "33";
  result = hash_table_put(hash_table, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(value3a == previous_value);
  assert(hash_table_has_key(hash_table, key3));
  assert(value3b == hash_table_get(hash_table, key3));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1] || key1 == keys[2]);
  assert(key2 == keys[0] || key2 == keys[1] || key2 == keys[2]);
  assert(key3 == keys[0] || key3 == keys[1] || key3 == keys[2]);
  free(keys);
  
  char const key4[] = "alpha";
  void const *removed_value = hash_table_remove(hash_table, key4);
  assert(NULL == removed_value);
  assert(3 == hash_table_count(hash_table));
  
  removed_value = hash_table_remove(hash_table, key3);
  assert(value3b == removed_value);
  assert(2 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1]);
  assert(key2 == keys[0] || key2 == keys[1]);
  free(keys);
  
  removed_value = hash_table_remove(hash_table, key1);
  assert(value1 == removed_value);
  assert(1 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key2 == keys[0]);
  free(keys);
  
  removed_value = hash_table_remove(hash_table, key2);
  assert(value2 == removed_value);
  assert(0 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  free(keys);
  
  hash_table_free(hash_table);
  return 0;
}


static bool
equal_string_keys(void const *first_key, void const *second_key)
{
  char const *first_string_key = first_key;
  char const *second_string_key = second_key;
  return 0 == strcmp(first_string_key, second_string_key);
}


static uint32_t
hash_string_key(void const *key)
{
  char const *string_key = key;
  uint32_t hash = 0;
  while (*string_key) {
    hash = 31 * hash + *string_key;
    ++string_key;
  }
  return hash;
}

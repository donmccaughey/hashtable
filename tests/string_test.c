#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"


static bool
equal_string_keys(void const *first_key, void const *second_key);

static size_t
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
  
  char *key1 = strdup("red");
  char *value1 = strdup("10");
  assert( ! hash_table_has_key(hash_table, key1));
  
  void const *previous_value;
  int result = hash_table_put(hash_table, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, key1));
  assert(hash_table_has_key(hash_table, "red"));
  assert(value1 == hash_table_get(hash_table, key1));
  assert(value1 == hash_table_get(hash_table, "red"));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0]);
  free(keys);
  
  char *key2 = strdup("green");
  char *value2 = strdup("20");
  assert( ! hash_table_has_key(hash_table, key2));
  
  result = hash_table_put(hash_table, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, key2));
  assert(hash_table_has_key(hash_table, "green"));
  assert(value2 == hash_table_get(hash_table, key2));
  assert(value2 == hash_table_get(hash_table, "green"));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1]);
  assert(key2 == keys[0] || key2 == keys[1]);
  free(keys);
  
  char *key3 = strdup("blue");
  char *value3a = strdup("30");
  assert( ! hash_table_has_key(hash_table, key3));
  
  result = hash_table_put(hash_table, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(NULL == previous_value);
  assert(hash_table_has_key(hash_table, key3));
  assert(hash_table_has_key(hash_table, "blue"));
  assert(value3a == hash_table_get(hash_table, key3));
  assert(value3a == hash_table_get(hash_table, "blue"));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1] || key1 == keys[2]);
  assert(key2 == keys[0] || key2 == keys[1] || key2 == keys[2]);
  assert(key3 == keys[0] || key3 == keys[1] || key3 == keys[2]);
  free(keys);
  
  char *value3b = strdup("33");
  result = hash_table_put(hash_table, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(value3a == previous_value);
  assert(hash_table_has_key(hash_table, key3));
  assert(hash_table_has_key(hash_table, "blue"));
  assert(value3b == hash_table_get(hash_table, key3));
  assert(value3b == hash_table_get(hash_table, "blue"));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1] || key1 == keys[2]);
  assert(key2 == keys[0] || key2 == keys[1] || key2 == keys[2]);
  assert(key3 == keys[0] || key3 == keys[1] || key3 == keys[2]);
  free(keys);
  
  char *key4 = strdup("alpha");
  void const *removed_value;
  hash_table_remove(hash_table, key4, &removed_value);
  assert(NULL == removed_value);
  assert(3 == hash_table_count(hash_table));
  
  hash_table_remove(hash_table, key3, &removed_value);
  assert(value3b == removed_value);
  assert(2 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1]);
  assert(key2 == keys[0] || key2 == keys[1]);
  free(keys);
  
  hash_table_remove(hash_table, key1, &removed_value);
  assert(value1 == removed_value);
  assert(1 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key2 == keys[0]);
  free(keys);
  
  hash_table_remove(hash_table, "green", &removed_value);
  assert(value2 == removed_value);
  assert(0 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  free(keys);
  
  free(key1);
  free(value1);
  free(key2);
  free(value2);
  free(key3);
  free(value3a);
  free(value3b);
  free(key4);
  
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


static size_t
hash_string_key(void const *key)
{
  char const *string_key = key;
  size_t hash = 0;
  while (*string_key) {
    hash = 31 * hash + *string_key;
    ++string_key;
  }
  return hash;
}

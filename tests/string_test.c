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
  union ht_value value1 = {.str_value=strdup("10"),};
  assert( ! hash_table_has_key(hash_table, key1));
  
  union ht_value previous_value;
  int result = hash_table_put(hash_table, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  assert(hash_table_has_key(hash_table, key1));
  union ht_value value;
  assert(0 == hash_table_get(hash_table, key1, &value));
  assert(value1.str_value == value.str_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0]);
  free(keys);
  
  char *key2 = strdup("green");
  union ht_value value2 = {.str_value=strdup("20"),};
  assert( ! hash_table_has_key(hash_table, key2));
  
  result = hash_table_put(hash_table, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(hash_table_has_key(hash_table, key2));
  assert(0 == hash_table_get(hash_table, key2, &value));
  assert(value2.str_value == value.str_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1]);
  assert(key2 == keys[0] || key2 == keys[1]);
  free(keys);
  
  char *key3 = strdup("blue");
  union ht_value value3a = {.str_value=strdup("30"),};
  assert( ! hash_table_has_key(hash_table, key3));
  
  result = hash_table_put(hash_table, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(hash_table_has_key(hash_table, key3));
  assert(0 == hash_table_get(hash_table, key3, &value));
  assert(value3a.str_value == value.str_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1] || key1 == keys[2]);
  assert(key2 == keys[0] || key2 == keys[1] || key2 == keys[2]);
  assert(key3 == keys[0] || key3 == keys[1] || key3 == keys[2]);
  free(keys);
  
  union ht_value value3b = {.str_value=strdup("33"),};
  result = hash_table_put(hash_table, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(value3a.str_value == previous_value.str_value);
  assert(hash_table_has_key(hash_table, key3));
  assert(0 == hash_table_get(hash_table, key3, &value));
  assert(value3b.str_value == value.str_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1] || key1 == keys[2]);
  assert(key2 == keys[0] || key2 == keys[1] || key2 == keys[2]);
  assert(key3 == keys[0] || key3 == keys[1] || key3 == keys[2]);
  free(keys);
  
  char *key4 = strdup("alpha");
  union ht_value removed_value;
  result = hash_table_remove(hash_table, key4, &removed_value);
  assert(-1 == result);
  assert(3 == hash_table_count(hash_table));
  
  result = hash_table_remove(hash_table, key3, &removed_value);
  assert(0 == result);
  assert(value3b.str_value == removed_value.str_value);
  assert(2 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key1 == keys[0] || key1 == keys[1]);
  assert(key2 == keys[0] || key2 == keys[1]);
  free(keys);
  
  result = hash_table_remove(hash_table, key1, &removed_value);
  assert(0 == result);
  assert(value1.str_value == removed_value.str_value);
  assert(1 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(key2 == keys[0]);
  free(keys);
  
  hash_table_remove(hash_table, "green", &removed_value);
  assert(0 == result);
  assert(value2.str_value == removed_value.str_value);
  assert(0 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  free(keys);
  
  free(key1);
  free(value1.str_value);
  free(key2);
  free(value2.str_value);
  free(key3);
  free(value3a.str_value);
  free(value3b.str_value);
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

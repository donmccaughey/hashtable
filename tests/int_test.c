#include <assert.h>
#include <stdlib.h>

#include "hash_table.h"


static bool
keys_contains_key(struct ht_key *keys, size_t count, struct ht_key key);

static bool
values_contains_value(union ht_value *values, size_t count, union ht_value value);


int
int_test(void)
{
  struct hash_table *hash_table = hash_table_alloc(10, ht_equal_int_keys);
  assert(hash_table);
  assert(10 == hash_table_capacity(hash_table));
  assert(0 == hash_table_count(hash_table));
  
  struct ht_key key1 = ht_int_key(1);
  union ht_value value1 = ht_int_value(10);
  assert(-1 == hash_table_get(hash_table, key1, NULL));
  
  union ht_value previous_value;
  int result = hash_table_put(hash_table, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hash_table_count(hash_table));
  union ht_value value;
  assert(0 == hash_table_get(hash_table, key1, &value));
  assert(value1.int_value == value.int_value);
  
  struct ht_key *keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(keys_contains_key(keys, hash_table_count(hash_table), key1));
  free(keys);
  
  union ht_value *values = hash_table_alloc_values(hash_table);
  assert(values);
  assert(values_contains_value(values, hash_table_count(hash_table), value1));
  free(values);
  
  struct ht_key key2 = ht_int_key(2);
  union ht_value value2 = ht_int_value(20);
  assert(-1 == hash_table_get(hash_table, key2, NULL));
  
  result = hash_table_put(hash_table, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hash_table_count(hash_table));
  assert(0 == hash_table_get(hash_table, key2, &value));
  assert(value2.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(keys_contains_key(keys, hash_table_count(hash_table), key1));
  assert(keys_contains_key(keys, hash_table_count(hash_table), key2));
  free(keys);
  
  values = hash_table_alloc_values(hash_table);
  assert(values);
  assert(values_contains_value(values, hash_table_count(hash_table), value1));
  assert(values_contains_value(values, hash_table_count(hash_table), value2));
  free(values);
  
  struct ht_key key3 = ht_int_key(3);
  union ht_value value3a = ht_int_value(30);
  assert(-1 == hash_table_get(hash_table, key3, NULL));
  
  result = hash_table_put(hash_table, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(0 == hash_table_get(hash_table, key3, &value));
  assert(value3a.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(keys_contains_key(keys, hash_table_count(hash_table), key1));
  assert(keys_contains_key(keys, hash_table_count(hash_table), key2));
  assert(keys_contains_key(keys, hash_table_count(hash_table), key3));
  free(keys);
  
  values = hash_table_alloc_values(hash_table);
  assert(values);
  assert(values_contains_value(values, hash_table_count(hash_table), value1));
  assert(values_contains_value(values, hash_table_count(hash_table), value2));
  assert(values_contains_value(values, hash_table_count(hash_table), value3a));
  free(values);
  
  union ht_value value3b = ht_int_value(33);
  result = hash_table_put(hash_table, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hash_table_count(hash_table));
  assert(value3a.int_value == previous_value.int_value);
  assert(0 == hash_table_get(hash_table, key3, &value));
  assert(value3b.int_value == value.int_value);
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(keys_contains_key(keys, hash_table_count(hash_table), key1));
  assert(keys_contains_key(keys, hash_table_count(hash_table), key2));
  assert(keys_contains_key(keys, hash_table_count(hash_table), key3));
  free(keys);
  
  values = hash_table_alloc_values(hash_table);
  assert(values);
  assert(values_contains_value(values, hash_table_count(hash_table), value1));
  assert(values_contains_value(values, hash_table_count(hash_table), value2));
  assert(values_contains_value(values, hash_table_count(hash_table), value3b));
  free(values);
  
  struct ht_key key4 = ht_int_key(4);
  union ht_value removed_value;
  result = hash_table_remove(hash_table, key4, &removed_value);
  assert(-1 == result);
  assert(3 == hash_table_count(hash_table));
  
  result = hash_table_remove(hash_table, key3, &removed_value);
  assert(0 == result);
  assert(value3b.int_value == removed_value.int_value);
  assert(2 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(keys_contains_key(keys, hash_table_count(hash_table), key1));
  assert(keys_contains_key(keys, hash_table_count(hash_table), key2));
  free(keys);
  
  values = hash_table_alloc_values(hash_table);
  assert(values);
  assert(values_contains_value(values, hash_table_count(hash_table), value1));
  assert(values_contains_value(values, hash_table_count(hash_table), value2));
  free(values);
  
  result = hash_table_remove(hash_table, key1, &removed_value);
  assert(0 == result);
  assert(value1.int_value == removed_value.int_value);
  assert(1 == hash_table_count(hash_table));
  
  keys = hash_table_alloc_keys(hash_table);
  assert(keys);
  assert(keys_contains_key(keys, hash_table_count(hash_table), key2));
  free(keys);
  
  values = hash_table_alloc_values(hash_table);
  assert(values);
  assert(values_contains_value(values, hash_table_count(hash_table), value2));
  free(values);
  
  result = hash_table_remove(hash_table, key2, &removed_value);
  assert(0 == result);
  assert(value2.int_value == removed_value.int_value);
  assert(0 == hash_table_count(hash_table));
  
  hash_table_free(hash_table);
  return 0;
}


static bool
keys_contains_key(struct ht_key *keys, size_t count, struct ht_key key)
{
  for (size_t i = 0; i < count; ++i) {
    if (ht_equal_int_keys(key, keys[i])) return true;
  }
  return false;
}


static bool
values_contains_value(union ht_value *values, size_t count, union ht_value value)
{
  for (size_t i = 0; i < count; ++i) {
    if (value.int_value == values[i].int_value) return true;
  }
  return false;
}

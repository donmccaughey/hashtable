#include <assert.h>

#include "hashtable.h"


static bool
keys_contains_key(struct ht_key *keys, size_t count, struct ht_key key);

static bool
values_contains_value(union ht_value *values, size_t count, union ht_value value);


int
int_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_int_keys);
  assert(hashtable);
  assert(10 == hashtable_capacity(hashtable));
  assert(0 == hashtable_count(hashtable));
  
  struct ht_key key1 = ht_int_key(1);
  union ht_value value1 = ht_int_value(10);
  assert(-1 == hashtable_get(hashtable, key1, NULL));
  
  union ht_value previous_value;
  int result = hashtable_put(hashtable, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hashtable_count(hashtable));
  union ht_value value;
  assert(0 == hashtable_get(hashtable, key1, &value));
  assert(value1.int_value == value.int_value);
  
  struct ht_key *keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  assert(keys_contains_key(keys, hashtable_count(hashtable), key1));
  free(keys);
  
  union ht_value *values = hashtable_alloc_values(hashtable);
  assert(values);
  assert(values_contains_value(values, hashtable_count(hashtable), value1));
  free(values);
  
  struct ht_key key2 = ht_int_key(2);
  union ht_value value2 = ht_int_value(20);
  assert(-1 == hashtable_get(hashtable, key2, NULL));
  
  result = hashtable_put(hashtable, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hashtable_count(hashtable));
  assert(0 == hashtable_get(hashtable, key2, &value));
  assert(value2.int_value == value.int_value);
  
  keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  assert(keys_contains_key(keys, hashtable_count(hashtable), key1));
  assert(keys_contains_key(keys, hashtable_count(hashtable), key2));
  free(keys);
  
  values = hashtable_alloc_values(hashtable);
  assert(values);
  assert(values_contains_value(values, hashtable_count(hashtable), value1));
  assert(values_contains_value(values, hashtable_count(hashtable), value2));
  free(values);
  
  struct ht_key key3 = ht_int_key(3);
  union ht_value value3a = ht_int_value(30);
  assert(-1 == hashtable_get(hashtable, key3, NULL));
  
  result = hashtable_put(hashtable, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hashtable_count(hashtable));
  assert(0 == hashtable_get(hashtable, key3, &value));
  assert(value3a.int_value == value.int_value);
  
  keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  assert(keys_contains_key(keys, hashtable_count(hashtable), key1));
  assert(keys_contains_key(keys, hashtable_count(hashtable), key2));
  assert(keys_contains_key(keys, hashtable_count(hashtable), key3));
  free(keys);
  
  values = hashtable_alloc_values(hashtable);
  assert(values);
  assert(values_contains_value(values, hashtable_count(hashtable), value1));
  assert(values_contains_value(values, hashtable_count(hashtable), value2));
  assert(values_contains_value(values, hashtable_count(hashtable), value3a));
  free(values);
  
  union ht_value value3b = ht_int_value(33);
  result = hashtable_put(hashtable, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hashtable_count(hashtable));
  assert(value3a.int_value == previous_value.int_value);
  assert(0 == hashtable_get(hashtable, key3, &value));
  assert(value3b.int_value == value.int_value);
  
  keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  assert(keys_contains_key(keys, hashtable_count(hashtable), key1));
  assert(keys_contains_key(keys, hashtable_count(hashtable), key2));
  assert(keys_contains_key(keys, hashtable_count(hashtable), key3));
  free(keys);
  
  values = hashtable_alloc_values(hashtable);
  assert(values);
  assert(values_contains_value(values, hashtable_count(hashtable), value1));
  assert(values_contains_value(values, hashtable_count(hashtable), value2));
  assert(values_contains_value(values, hashtable_count(hashtable), value3b));
  free(values);
  
  struct ht_key key4 = ht_int_key(4);
  union ht_value removed_value;
  result = hashtable_remove(hashtable, key4, &removed_value);
  assert(-1 == result);
  assert(3 == hashtable_count(hashtable));
  
  result = hashtable_remove(hashtable, key3, &removed_value);
  assert(0 == result);
  assert(value3b.int_value == removed_value.int_value);
  assert(2 == hashtable_count(hashtable));
  
  keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  assert(keys_contains_key(keys, hashtable_count(hashtable), key1));
  assert(keys_contains_key(keys, hashtable_count(hashtable), key2));
  free(keys);
  
  values = hashtable_alloc_values(hashtable);
  assert(values);
  assert(values_contains_value(values, hashtable_count(hashtable), value1));
  assert(values_contains_value(values, hashtable_count(hashtable), value2));
  free(values);
  
  result = hashtable_remove(hashtable, key1, &removed_value);
  assert(0 == result);
  assert(value1.int_value == removed_value.int_value);
  assert(1 == hashtable_count(hashtable));
  
  keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  assert(keys_contains_key(keys, hashtable_count(hashtable), key2));
  free(keys);
  
  values = hashtable_alloc_values(hashtable);
  assert(values);
  assert(values_contains_value(values, hashtable_count(hashtable), value2));
  free(values);
  
  result = hashtable_remove(hashtable, key2, &removed_value);
  assert(0 == result);
  assert(value2.int_value == removed_value.int_value);
  assert(0 == hashtable_count(hashtable));
  
  hashtable_free(hashtable);
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

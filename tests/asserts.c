#include "asserts.h"

#include <assert.h>


void
assert_keys_and_values(struct hashtable *hashtable,
                       struct ht_key expected_keys[],
                       union ht_value expected_values[],
                       size_t expected_count,
                       ht_equal_keys_func equal_keys,
                       ht_equal_values_func equal_values)
{
  struct ht_key *keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  
  union ht_value *values = hashtable_alloc_values(hashtable);
  assert(values);
  
  size_t iterator = 0;
  struct ht_key next_key;
  union ht_value next_value;
  
  for (size_t i = 0; i < expected_count; ++i) {
    assert(keys_contains_key(keys,
                             hashtable_count(hashtable),
                             expected_keys[i],
                             equal_keys));
    assert(values_contains_value(values,
                                 hashtable_count(hashtable),
                                 expected_values[i],
                                 equal_values));
    
    assert(hashtable_next(hashtable, &iterator, &next_key, &next_value));
    assert(keys_contains_key(keys,
                             hashtable_count(hashtable),
                             next_key,
                             equal_keys));
    assert(values_contains_value(values,
                                 hashtable_count(hashtable),
                                 next_value,
                                 equal_values));
    
    assert(keys_contains_key(expected_keys,
                             expected_count,
                             next_key,
                             equal_keys));
    assert(values_contains_value(expected_values,
                                 expected_count,
                                 next_value,
                                 equal_values));
  }
  
  assert( ! hashtable_next(hashtable, &iterator, NULL, NULL));
  assert( ! hashtable_next(hashtable, &iterator, NULL, NULL));
  
  free(keys);
  free(values);
}


bool
keys_contains_key(struct ht_key *keys,
                  size_t count,
                  struct ht_key key,
                  ht_equal_keys_func equal_keys)
{
  for (size_t i = 0; i < count; ++i) {
    if (equal_keys(key, keys[i])) return true;
  }
  return false;
}


bool
values_contains_value(union ht_value *values,
                      size_t count,
                      union ht_value value,
                      ht_equal_values_func equal_values)
{
  for (size_t i = 0; i < count; ++i) {
    if (equal_values(value, values[i])) return true;
  }
  return false;
}

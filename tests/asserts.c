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
  
  struct ht_entry *entries = hashtable_alloc_entries(hashtable);
  assert(entries);
  
  size_t iterator = 0;
  struct ht_entry const *next_entry;
  
  for (size_t i = 0; i < expected_count; ++i) {
    assert(entries_contains_key_and_value(entries,
                                          hashtable->count,
                                          expected_keys[i],
                                          equal_keys,
                                          expected_values[i],
                                          equal_values));
    assert(keys_contains_key(keys,
                             hashtable->count,
                             expected_keys[i],
                             equal_keys));
    assert(values_contains_value(values,
                                 hashtable->count,
                                 expected_values[i],
                                 equal_values));
    
    next_entry = hashtable_next(hashtable, &iterator);
    assert(next_entry);
    
    assert(entries_contains_key_and_value(entries,
                                          hashtable->count,
                                          next_entry->key,
                                          equal_keys,
                                          next_entry->value,
                                          equal_values));
    assert(keys_contains_key(keys,
                             hashtable->count,
                             next_entry->key,
                             equal_keys));
    assert(values_contains_value(values,
                                 hashtable->count,
                                 next_entry->value,
                                 equal_values));
    
    assert(keys_contains_key(expected_keys,
                             expected_count,
                             next_entry->key,
                             equal_keys));
    assert(values_contains_value(expected_values,
                                 expected_count,
                                 next_entry->value,
                                 equal_values));
  }
  
  assert( ! hashtable_next(hashtable, &iterator));
  assert( ! hashtable_next(hashtable, &iterator));
  
  free(keys);
  free(values);
  free(entries);
}


bool
entries_contains_key_and_value(struct ht_entry *entries,
                               size_t count,
                               struct ht_key key,
                               ht_equal_keys_func equal_keys,
                               union ht_value value,
                               ht_equal_values_func equal_values)
{
  for (size_t i = 0; i < count; ++i) {
    if (   equal_keys(key, entries[i].key)
        && equal_values(value, entries[i].value))
    {
      return true;
    }
  }
  return false;
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

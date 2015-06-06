#include "asserts.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


void
assert_contains_long_values_for_long_keys_in_sequence(struct hashtable const *hashtable,
                                                      long key_start,
                                                      long value_start,
                                                      int count)
{
  long end = key_start + count;
  for (long key = key_start, value = value_start; key < end; ++key, ++value) {
    struct ht_entry const *entry = hashtable_get(hashtable, ht_long_key(key));
    assert(entry);
    assert(value == entry->value.long_value);
  }
}


void
assert_contains_str_values_for_str_keys_in_sequence(struct hashtable const *hashtable,
                                                    int key_start,
                                                    int value_start,
                                                    int count)
{
  int end = key_start + count;
  for (int key = key_start, value = value_start; key < end; ++key, ++value) {
    char *key_str;
    int chars_printed = asprintf(&key_str, "%i", key);
    assert(chars_printed > 0);
    
    char *value_str;
    chars_printed = asprintf(&value_str, "%i", value);
    assert(chars_printed > 0);
    
    struct ht_entry const *entry = hashtable_get(hashtable, ht_str_key(key_str));
    assert(entry);
    assert(0 == strcmp(value_str, entry->value.str_value));
    
    free(value_str);
    free(key_str);
  }
}


void
assert_keys_and_values(struct hashtable const *hashtable,
                       struct ht_key expected_keys[],
                       union ht_value expected_values[],
                       int expected_count,
                       ht_equal_keys_func equal_keys,
                       equal_values_func equal_values)
{
  struct ht_key *keys = hashtable_alloc_keys(hashtable);
  assert(keys);
  
  union ht_value *values = hashtable_alloc_values(hashtable);
  assert(values);
  
  struct ht_entry *entries = hashtable_alloc_entries(hashtable);
  assert(entries);
  
  int iterator = 0;
  struct ht_entry const *next_entry;
  
  for (int i = 0; i < expected_count; ++i) {
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
entries_contains_key_and_value(struct ht_entry const *entries,
                               int count,
                               struct ht_key key,
                               ht_equal_keys_func equal_keys,
                               union ht_value value,
                               equal_values_func equal_values)
{
  for (int i = 0; i < count; ++i) {
    if (   equal_keys(key, entries[i].key)
        && equal_values(value, entries[i].value))
    {
      return true;
    }
  }
  return false;
}


bool
keys_contains_key(struct ht_key const *keys,
                  int count,
                  struct ht_key key,
                  ht_equal_keys_func equal_keys)
{
  for (int i = 0; i < count; ++i) {
    if (equal_keys(key, keys[i])) return true;
  }
  return false;
}


bool
values_contains_value(union ht_value const *values,
                      int count,
                      union ht_value value,
                      equal_values_func equal_values)
{
  for (int i = 0; i < count; ++i) {
    if (equal_values(value, values[i])) return true;
  }
  return false;
}

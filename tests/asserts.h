#ifndef TESTS_ASSERTS_H_INCLUDED
#define TESTS_ASSERTS_H_INCLUDED


#include "hashtable.h"


void
assert_keys_and_values(struct hashtable *hashtable,
                       struct ht_key expected_keys[],
                       union ht_value expected_values[],
                       int expected_count,
                       ht_equal_keys_func equal_keys,
                       ht_equal_values_func equal_values);

bool
entries_contains_key_and_value(struct ht_entry *entries,
                               int count,
                               struct ht_key key,
                               ht_equal_keys_func equal_keys,
                               union ht_value value,
                               ht_equal_values_func equal_values);

bool
keys_contains_key(struct ht_key *keys,
                  int count,
                  struct ht_key key,
                  ht_equal_keys_func equal_keys);

bool
values_contains_value(union ht_value *values,
                      int count,
                      union ht_value value,
                      ht_equal_values_func equal_values);


#endif

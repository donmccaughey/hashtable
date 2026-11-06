#ifndef TESTS_ASSERTS_H_INCLUDED
#define TESTS_ASSERTS_H_INCLUDED


#include "hashtable.h"


typedef bool
equal_values_func(union ht_value first, union ht_value second);


void
assert_contains_long_values_for_long_keys_in_sequence(struct hashtable const *hashtable,
                                                      long key_start,
                                                      long value_start,
                                                      size_t count);

void
assert_contains_str_values_for_str_keys_in_sequence(struct hashtable const *hashtable,
                                                    size_t key_start,
                                                    size_t value_start,
                                                    size_t count);

void
assert_keys_and_values(struct hashtable const *hashtable,
                       struct ht_key expected_keys[],
                       union ht_value expected_values[],
                       size_t expected_count,
                       ht_equal_keys_func equal_keys,
                       equal_values_func equal_values);

bool
entries_contains_key_and_value(struct ht_entry const *entries,
                               size_t count,
                               struct ht_key key,
                               ht_equal_keys_func equal_keys,
                               union ht_value value,
                               equal_values_func equal_values);

bool
keys_contains_key(struct ht_key const *keys,
                  size_t count,
                  struct ht_key key,
                  ht_equal_keys_func equal_keys);

bool
values_contains_value(union ht_value const *values,
                      size_t count,
                      union ht_value value,
                      equal_values_func equal_values);


#endif

#include <assert.h>

#include "asserts.h"
#include "hashtable.h"


int
string_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_str_keys);
  assert(hashtable);
  assert(10 == hashtable->capacity);
  assert(0 == hashtable->count);
  assert_keys_and_values(hashtable,
                         NULL,
                         NULL,
                         0,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key1 = ht_str_key(strdup("red"));
  assert(key1.value.str_value);
  union ht_value value1 = ht_str_value(strdup("10"));
  assert(value1.str_value);
  assert( ! hashtable_get(hashtable, key1));
  
  bool had_entry;
  struct ht_entry previous_entry;
  int result = hashtable_set(hashtable, key1, value1, &had_entry, &previous_entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(1 == hashtable->count);
  struct ht_entry const *entry;
  entry = hashtable_get(hashtable, key1);
  assert(entry);
  assert(value1.str_value == entry->value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, },
                         (union ht_value[]){ value1, },
                         1,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key2 = ht_str_key(strdup("green"));
  assert(key2.value.str_value);
  union ht_value value2 = ht_str_value(strdup("20"));
  assert(value2.str_value);
  assert( ! hashtable_get(hashtable, key2));
  
  result = hashtable_set(hashtable, key2, value2, &had_entry, &previous_entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(2 == hashtable->count);
  entry = hashtable_get(hashtable, key2);
  assert(entry);
  assert(value2.str_value == entry->value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key3 = ht_str_key(strdup("blue"));
  assert(key3.value.str_value);
  union ht_value value3a = ht_str_value(strdup("30"));
  assert(value3a.str_value);
  assert( ! hashtable_get(hashtable, key3));
  
  result = hashtable_set(hashtable, key3, value3a, &had_entry, &previous_entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(3 == hashtable->count);
  entry = hashtable_get(hashtable, key3);
  assert(entry);
  assert(value3a.str_value == entry->value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3a, },
                         3,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  union ht_value value3b = ht_str_value(strdup("33"));
  assert(value3b.str_value);
  result = hashtable_set(hashtable, key3, value3b, &had_entry, &previous_entry);
  assert(0 == result);
  assert(had_entry);
  assert(3 == hashtable->count);
  assert(value3a.str_value == previous_entry.value.str_value);
  entry = hashtable_get(hashtable, key3);
  assert(entry);
  assert(value3b.str_value == entry->value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3b, },
                         3,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key4 = ht_str_key(strdup("alpha"));
  assert(key4.value.str_value);
  result = hashtable_remove(hashtable, key4, NULL);
  assert(-1 == result);
  assert(3 == hashtable->count);
  
  
  struct ht_entry removed_entry;
  result = hashtable_remove(hashtable, key3, &removed_entry);
  assert(0 == result);
  assert(value3b.str_value == removed_entry.value.str_value);
  assert(2 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  result = hashtable_remove(hashtable, key1, &removed_entry);
  assert(0 == result);
  assert(value1.str_value == removed_entry.value.str_value);
  assert(1 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key2, },
                         (union ht_value[]){ value2, },
                         1,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  hashtable_remove(hashtable, key2, &removed_entry);
  assert(0 == result);
  assert(value2.str_value == removed_entry.value.str_value);
  assert(0 == hashtable->count);
  assert_keys_and_values(hashtable,
                         NULL,
                         NULL,
                         0,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  free(key1.value.str_value);
  free(value1.str_value);
  free(key2.value.str_value);
  free(value2.str_value);
  free(key3.value.str_value);
  free(value3a.str_value);
  free(value3b.str_value);
  free(key4.value.str_value);
  
  hashtable_free(hashtable);
  return 0;
}

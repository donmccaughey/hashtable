#include <assert.h>

#include "asserts.h"
#include "hashtable.h"


static bool
equal_long_values(union ht_value first, union ht_value second)
{
  return first.long_value == second.long_value;
}


void
long_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_long_keys);
  assert(hashtable);
  assert(10 == hashtable->capacity);
  assert(0 == hashtable->count);
  assert_keys_and_values(hashtable,
                         NULL,
                         NULL,
                         0,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  struct ht_key key1 = ht_long_key(1);
  union ht_value value1 = ht_long_value(10);
  assert( ! hashtable_get(hashtable, key1));
  
  bool had_entry;
  struct ht_entry previous_entry;
  int result = hashtable_set(hashtable, key1, value1, &had_entry, &previous_entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(1 == hashtable->count);
  struct ht_entry const *entry = hashtable_get(hashtable, key1);
  assert(entry);
  assert(value1.long_value == entry->value.long_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, },
                         (union ht_value[]){ value1, },
                         1,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  struct ht_key key2 = ht_long_key(2);
  union ht_value value2 = ht_long_value(20);
  assert( ! hashtable_get(hashtable, key2));
  
  result = hashtable_set(hashtable, key2, value2, &had_entry, &previous_entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(2 == hashtable->count);
  entry = hashtable_get(hashtable, key2);
  assert(entry);
  assert(value2.long_value == entry->value.long_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  struct ht_key key3 = ht_long_key(3);
  union ht_value value3a = ht_long_value(30);
  assert( ! hashtable_get(hashtable, key3));
  
  result = hashtable_set(hashtable, key3, value3a, &had_entry, &previous_entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(3 == hashtable->count);
  entry = hashtable_get(hashtable, key3);
  assert(entry);
  assert(value3a.long_value == entry->value.long_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3a, },
                         3,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  union ht_value value3b = ht_long_value(33);
  result = hashtable_set(hashtable, key3, value3b, &had_entry, &previous_entry);
  assert(0 == result);
  assert(had_entry);
  assert(3 == hashtable->count);
  assert(value3a.long_value == previous_entry.value.long_value);
  entry = hashtable_get(hashtable, key3);
  assert(entry);
  assert(value3b.long_value == entry->value.long_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3b, },
                         3,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  struct ht_key key4 = ht_long_key(4);
  result = hashtable_remove(hashtable, key4, NULL);
  assert(-1 == result);
  assert(3 == hashtable->count);
  
  
  struct ht_entry removed_entry;
  result = hashtable_remove(hashtable, key3, &removed_entry);
  assert(0 == result);
  assert(value3b.long_value == removed_entry.value.long_value);
  assert(2 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_long_keys,
                         equal_long_values);

  
  result = hashtable_remove(hashtable, key1, &removed_entry);
  assert(0 == result);
  assert(value1.long_value == removed_entry.value.long_value);
  assert(1 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key2, },
                         (union ht_value[]){ value2, },
                         1,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  result = hashtable_remove(hashtable, key2, &removed_entry);
  assert(0 == result);
  assert(value2.long_value == removed_entry.value.long_value);
  assert(0 == hashtable->count);
  assert_keys_and_values(hashtable,
                         NULL,
                         NULL,
                         0,
                         ht_equal_long_keys,
                         equal_long_values);
  
  
  hashtable_free(hashtable, NULL);
}

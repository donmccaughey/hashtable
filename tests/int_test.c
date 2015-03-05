#include <assert.h>

#include "asserts.h"
#include "hashtable.h"


int
int_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_int_keys);
  assert(hashtable);
  assert(10 == hashtable->capacity);
  assert(0 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){},
                         (union ht_value[]){},
                         0,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key1 = ht_int_key(1);
  union ht_value value1 = ht_int_value(10);
  assert(-1 == hashtable_get(hashtable, key1, NULL));
  
  bool had_entry;
  struct ht_entry entry;
  int result = hashtable_set(hashtable, key1, value1, &had_entry, &entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(1 == hashtable->count);
  assert(0 == hashtable_get(hashtable, key1, &entry));
  assert(value1.int_value == entry.value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, },
                         (union ht_value[]){ value1, },
                         1,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key2 = ht_int_key(2);
  union ht_value value2 = ht_int_value(20);
  assert(-1 == hashtable_get(hashtable, key2, NULL));
  
  result = hashtable_set(hashtable, key2, value2, &had_entry, &entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(2 == hashtable->count);
  assert(0 == hashtable_get(hashtable, key2, &entry));
  assert(value2.int_value == entry.value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key3 = ht_int_key(3);
  union ht_value value3a = ht_int_value(30);
  assert(-1 == hashtable_get(hashtable, key3, NULL));
  
  result = hashtable_set(hashtable, key3, value3a, &had_entry, &entry);
  assert(0 == result);
  assert( ! had_entry);
  assert(3 == hashtable->count);
  assert(0 == hashtable_get(hashtable, key3, &entry));
  assert(value3a.int_value == entry.value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3a, },
                         3,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  union ht_value value3b = ht_int_value(33);
  result = hashtable_set(hashtable, key3, value3b, &had_entry, &entry);
  assert(0 == result);
  assert(had_entry);
  assert(3 == hashtable->count);
  assert(value3a.int_value == entry.value.int_value);
  assert(0 == hashtable_get(hashtable, key3, &entry));
  assert(value3b.int_value == entry.value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3b, },
                         3,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key4 = ht_int_key(4);
  result = hashtable_remove(hashtable, key4, NULL);
  assert(-1 == result);
  assert(3 == hashtable->count);
  
  
  struct ht_entry removed_entry;
  result = hashtable_remove(hashtable, key3, &removed_entry);
  assert(0 == result);
  assert(value3b.int_value == removed_entry.value.int_value);
  assert(2 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_int_keys,
                         ht_equal_int_values);

  
  result = hashtable_remove(hashtable, key1, &removed_entry);
  assert(0 == result);
  assert(value1.int_value == removed_entry.value.int_value);
  assert(1 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key2, },
                         (union ht_value[]){ value2, },
                         1,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  result = hashtable_remove(hashtable, key2, &removed_entry);
  assert(0 == result);
  assert(value2.int_value == removed_entry.value.int_value);
  assert(0 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){},
                         (union ht_value[]){},
                         0,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  hashtable_free(hashtable);
  return 0;
}

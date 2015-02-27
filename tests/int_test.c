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
  
  union ht_value previous_value;
  int result = hashtable_put(hashtable, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hashtable->count);
  union ht_value value;
  assert(0 == hashtable_get(hashtable, key1, &value));
  assert(value1.int_value == value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, },
                         (union ht_value[]){ value1, },
                         1,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key2 = ht_int_key(2);
  union ht_value value2 = ht_int_value(20);
  assert(-1 == hashtable_get(hashtable, key2, NULL));
  
  result = hashtable_put(hashtable, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hashtable->count);
  assert(0 == hashtable_get(hashtable, key2, &value));
  assert(value2.int_value == value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key3 = ht_int_key(3);
  union ht_value value3a = ht_int_value(30);
  assert(-1 == hashtable_get(hashtable, key3, NULL));
  
  result = hashtable_put(hashtable, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hashtable->count);
  assert(0 == hashtable_get(hashtable, key3, &value));
  assert(value3a.int_value == value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3a, },
                         3,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  union ht_value value3b = ht_int_value(33);
  result = hashtable_put(hashtable, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hashtable->count);
  assert(value3a.int_value == previous_value.int_value);
  assert(0 == hashtable_get(hashtable, key3, &value));
  assert(value3b.int_value == value.int_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3b, },
                         3,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  struct ht_key key4 = ht_int_key(4);
  union ht_value removed_value;
  result = hashtable_remove(hashtable, key4, &removed_value);
  assert(-1 == result);
  assert(3 == hashtable->count);
  
  
  result = hashtable_remove(hashtable, key3, &removed_value);
  assert(0 == result);
  assert(value3b.int_value == removed_value.int_value);
  assert(2 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_int_keys,
                         ht_equal_int_values);

  
  result = hashtable_remove(hashtable, key1, &removed_value);
  assert(0 == result);
  assert(value1.int_value == removed_value.int_value);
  assert(1 == hashtable->count);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key2, },
                         (union ht_value[]){ value2, },
                         1,
                         ht_equal_int_keys,
                         ht_equal_int_values);
  
  
  result = hashtable_remove(hashtable, key2, &removed_value);
  assert(0 == result);
  assert(value2.int_value == removed_value.int_value);
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

#include <assert.h>

#include "asserts.h"
#include "hashtable.h"


int
string_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_str_keys);
  assert(hashtable);
  assert(10 == hashtable_capacity(hashtable));
  assert(0 == hashtable_count(hashtable));
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){},
                         (union ht_value[]){},
                         0,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key1 = ht_alloc_str_key("red");
  assert(key1.value.str_value);
  union ht_value value1 = ht_alloc_str_value("10");
  assert(value1.str_value);
  assert(-1 == hashtable_get(hashtable, key1, NULL));
  
  union ht_value previous_value;
  int result = hashtable_put(hashtable, key1, value1, &previous_value);
  assert(0 == result);
  assert(1 == hashtable_count(hashtable));
  union ht_value value;
  assert(0 == hashtable_get(hashtable, key1, &value));
  assert(value1.str_value == value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, },
                         (union ht_value[]){ value1, },
                         1,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key2 = ht_alloc_str_key("green");
  assert(key2.value.str_value);
  union ht_value value2 = ht_alloc_str_value("20");
  assert(value2.str_value);
  assert(-1 == hashtable_get(hashtable, key2, NULL));
  
  result = hashtable_put(hashtable, key2, value2, &previous_value);
  assert(0 == result);
  assert(2 == hashtable_count(hashtable));
  assert(0 == hashtable_get(hashtable, key2, &value));
  assert(value2.str_value == value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key3 = ht_alloc_str_key("blue");
  assert(key3.value.str_value);
  union ht_value value3a = ht_alloc_str_value("30");
  assert(value3a.str_value);
  assert(-1 == hashtable_get(hashtable, key3, NULL));
  
  result = hashtable_put(hashtable, key3, value3a, &previous_value);
  assert(0 == result);
  assert(3 == hashtable_count(hashtable));
  assert(0 == hashtable_get(hashtable, key3, &value));
  assert(value3a.str_value == value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3a, },
                         3,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  union ht_value value3b = ht_alloc_str_value("33");
  assert(value3b.str_value);
  result = hashtable_put(hashtable, key3, value3b, &previous_value);
  assert(0 == result);
  assert(3 == hashtable_count(hashtable));
  assert(value3a.str_value == previous_value.str_value);
  assert(0 == hashtable_get(hashtable, key3, &value));
  assert(value3b.str_value == value.str_value);
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, key3, },
                         (union ht_value[]){ value1, value2, value3b, },
                         3,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  struct ht_key key4 = ht_alloc_str_key("alpha");
  assert(key4.value.str_value);
  union ht_value removed_value;
  result = hashtable_remove(hashtable, key4, &removed_value);
  assert(-1 == result);
  assert(3 == hashtable_count(hashtable));
  
  
  result = hashtable_remove(hashtable, key3, &removed_value);
  assert(0 == result);
  assert(value3b.str_value == removed_value.str_value);
  assert(2 == hashtable_count(hashtable));
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key1, key2, },
                         (union ht_value[]){ value1, value2, },
                         2,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  result = hashtable_remove(hashtable, key1, &removed_value);
  assert(0 == result);
  assert(value1.str_value == removed_value.str_value);
  assert(1 == hashtable_count(hashtable));
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){ key2, },
                         (union ht_value[]){ value2, },
                         1,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  hashtable_remove(hashtable, key2, &removed_value);
  assert(0 == result);
  assert(value2.str_value == removed_value.str_value);
  assert(0 == hashtable_count(hashtable));
  assert_keys_and_values(hashtable,
                         (struct ht_key[]){},
                         (union ht_value[]){},
                         0,
                         ht_equal_str_keys,
                         ht_equal_str_values);
  
  
  ht_free_str_key(key1);
  ht_free_str_value(value1);
  ht_free_str_key(key2);
  ht_free_str_value(value2);
  ht_free_str_key(key3);
  ht_free_str_value(value3a);
  ht_free_str_value(value3b);
  ht_free_str_key(key4);
  
  hashtable_free(hashtable);
  return 0;
}

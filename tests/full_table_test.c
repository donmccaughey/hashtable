#include <assert.h>

#include "hashtable.h"


int
full_table_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_int_keys);
  assert(hashtable);
  assert(10 == hashtable->capacity);
  assert(0 == hashtable->count);
  
  for (int i = 0; i < 10; ++i) {
    assert(0 == hashtable_put(hashtable, ht_int_key(i), ht_int_value(i), NULL));
  }
  assert(10 == hashtable->count);
  
  assert(-1 == hashtable_put(hashtable, ht_int_key(11), ht_int_value(11), NULL));
  
  for (int i = 0; i < 10; ++i) {
    union ht_value value;
    assert(0 == hashtable_get(hashtable, ht_int_key(i), &value));
    assert(i == value.int_value);
  }
  
  for (int i = 0; i < 10; ++i) {
    union ht_value previous_value;
    assert(0 == hashtable_put(hashtable, ht_int_key(i), ht_int_value(i * i), &previous_value));
    assert(i == previous_value.int_value);
  }
  
  for (int i = 0; i < 10; ++i) {
    union ht_value value;
    assert(0 == hashtable_get(hashtable, ht_int_key(i), &value));
    assert(i * i == value.int_value);
  }
  
  assert(0 == hashtable_remove(hashtable, ht_int_key(0), NULL));
  assert(9 == hashtable->count);
  assert(0 == hashtable_put(hashtable, ht_int_key(10), ht_int_value(10 * 10), NULL));
  assert(10 == hashtable->count);
  assert(-1 == hashtable_put(hashtable, ht_int_key(0), ht_int_value(0 * 0), NULL));
  
  for (int i = 1; i < 11; ++i) {
    union ht_value previous_value;
    assert(0 == hashtable_remove(hashtable, ht_int_key(i), &previous_value));
    assert(i * i == previous_value.int_value);
  }
  assert(0 == hashtable->count);
  
  hashtable_free(hashtable);
  return 0;
}

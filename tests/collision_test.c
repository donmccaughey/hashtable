#include <assert.h>

#include "hashtable.h"


static struct ht_key
colliding_long_key(long value)
{
  return (struct ht_key){
    .hash = 3,
    .value = ht_long_value(value)
  };
}


void
collision_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_long_keys);
  assert(hashtable);
  assert(10 == hashtable->capacity);
  assert(0 == hashtable->count);
  
  for (int i = 0; i < 7; ++i) {
    assert(0 == hashtable_set(hashtable, colliding_long_key(i), ht_long_value(i), NULL, NULL));
  }
  assert(7 == hashtable->count);
  
  for (int i = 0; i < 7; ++i) {
    assert(hashtable_get(hashtable, colliding_long_key(i)));
    assert(i == hashtable_get(hashtable, colliding_long_key(i))->value.long_value);
  }
  
  // remove an entry
  assert(0 == hashtable_remove(hashtable, colliding_long_key(3), NULL));
  assert(6 == hashtable->count);
  
  for (int i = 0; i < 7; ++i) {
    if (3 == i) {
      assert( ! hashtable_get(hashtable, colliding_long_key(i)));
    } else {
      assert(hashtable_get(hashtable, colliding_long_key(i)));
      assert(i == hashtable_get(hashtable, colliding_long_key(i))->value.long_value);
    }
  }
  
  // add the entry again
  assert(0 == hashtable_set(hashtable, colliding_long_key(3), ht_long_value(3), NULL, NULL));
  assert(7 == hashtable->count);
  
  assert(hashtable_get(hashtable, colliding_long_key(3)));
  assert(3 == hashtable_get(hashtable, colliding_long_key(3))->value.long_value);
  
  // fill the hash table to capacity
  for (int i = 7; i < 10; ++i) {
    assert(0 == hashtable_set(hashtable, colliding_long_key(i), ht_long_value(i), NULL, NULL));
  }
  assert(10 == hashtable->count);
  
  for (int i = 0; i < 10; ++i) {
    assert(hashtable_get(hashtable, colliding_long_key(i)));
    assert(i == hashtable_get(hashtable, colliding_long_key(i))->value.long_value);
  }
  
  // remove the first four items
  for (int i = 0; i < 4; ++i) {
    assert(0 == hashtable_remove(hashtable, colliding_long_key(i), NULL));
  }
  assert(6 == hashtable->count);
  
  for (int i = 4; i < 10; ++i) {
    assert(hashtable_get(hashtable, colliding_long_key(i)));
    assert(i == hashtable_get(hashtable, colliding_long_key(i))->value.long_value);
  }
  
  hashtable_free(hashtable, NULL);
}

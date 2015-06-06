#include <assert.h>

#include "hashtable.h"


void
full_table_test(void)
{
  struct hashtable *hashtable = hashtable_alloc(10, ht_equal_long_keys);
  assert(hashtable);
  assert(10 == hashtable->capacity);
  assert(0 == hashtable->count);
  
  for (int i = 0; i < 10; ++i) {
    assert(0 == hashtable_set(hashtable, ht_long_key(i), ht_long_value(i), NULL, NULL));
  }
  assert(10 == hashtable->count);
  
  assert(-1 == hashtable_set(hashtable, ht_long_key(11), ht_long_value(11), NULL, NULL));
  
  for (int i = 0; i < 10; ++i) {
    struct ht_entry const *entry = hashtable_get(hashtable, ht_long_key(i));
    assert(i == entry->value.long_value);
  }
  
  for (int i = 0; i < 10; ++i) {
    bool had_entry;
    struct ht_entry entry;
    assert(0 == hashtable_set(hashtable, ht_long_key(i), ht_long_value(i * i), &had_entry, &entry));
    assert(had_entry);
    assert(i == entry.value.long_value);
  }
  
  for (int i = 0; i < 10; ++i) {
    struct ht_entry const *entry = hashtable_get(hashtable, ht_long_key(i));
    assert(i * i == entry->value.long_value);
  }
  
  assert(0 == hashtable_remove(hashtable, ht_long_key(0), NULL));
  assert(9 == hashtable->count);
  assert(0 == hashtable_set(hashtable, ht_long_key(10), ht_long_value(10 * 10), NULL, NULL));
  assert(10 == hashtable->count);
  assert(-1 == hashtable_set(hashtable, ht_long_key(0), ht_long_value(0 * 0), NULL, NULL));
  
  for (int i = 1; i < 11; ++i) {
    struct ht_entry entry;
    assert(0 == hashtable_remove(hashtable, ht_long_key(i), &entry));
    assert(i * i == entry.value.long_value);
  }
  assert(0 == hashtable->count);
  
  hashtable_free(hashtable, NULL);
}

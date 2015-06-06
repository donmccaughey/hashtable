#include <assert.h>
#include <stdio.h>

#include "asserts.h"
#include "hashtable.h"


static int copy_count = 0;
static int free_count = 0;


static int
copy_str_entry(struct hashtable *destination,
               struct ht_entry *destination_entry,
               struct ht_entry source_entry)
{
  ((void)destination);
  ++copy_count;
  destination_entry->key.hash = source_entry.key.hash;
  destination_entry->key.value.str_value = strdup(source_entry.key.value.str_value);
  assert(destination_entry->key.value.str_value);
  destination_entry->value.str_value = strdup(source_entry.value.str_value);
  assert(destination_entry->value.str_value);
  return 0;
}


static void
free_str_entry(struct hashtable *destination,
               struct ht_entry destination_entry)
{
  ((void)destination);
  ++free_count;
  free(destination_entry.key.value.str_value);
  free(destination_entry.value.str_value);
}


static struct hashtable *
alloc_and_fill_hashtable_with_longs(int capacity,
                                    int count,
                                    long key_start,
                                    long value_start)
{
  struct hashtable *hashtable = hashtable_alloc(capacity, ht_equal_long_keys);
  assert(hashtable);
  assert(capacity == hashtable->capacity);
  for (int i = 0; i < count; ++i) {
    assert(0 == hashtable_set(hashtable,
                              ht_long_key(key_start + i),
                              ht_long_value(value_start + i),
                              NULL,
                              NULL));
  }
  assert(count == hashtable->count);
  return hashtable;
}


static struct hashtable *
alloc_and_fill_hashtable_with_strings(int capacity,
                                      int count,
                                      int key_start,
                                      int value_start)
{
  struct hashtable *hashtable = hashtable_alloc(capacity, ht_equal_str_keys);
  assert(hashtable);
  assert(capacity == hashtable->capacity);
  for (int i = 0; i < count; ++i) {
    char *key;
    int chars_printed = asprintf(&key, "%i", key_start + i);
    assert(chars_printed > 0);
    
    char *value;
    chars_printed = asprintf(&value, "%i", value_start + i);
    assert(chars_printed > 0);
    
    assert(0 == hashtable_set(hashtable,
                              ht_str_key(key),
                              ht_str_value(value),
                              NULL,
                              NULL));
  }
  assert(count == hashtable->count);
  return hashtable;
}


static void
test_long_source_larger_than_capacity_for_empty_destination(void)
{
  struct hashtable *source = alloc_and_fill_hashtable_with_longs(30, 16, 0, 1000);
  struct hashtable *destination = alloc_and_fill_hashtable_with_longs(15, 0, 0, 0);
  
  int result = hashtable_update(destination, source, NULL, NULL);
  
  assert(-1 == result);
  assert(0 == destination->count);
  
  hashtable_free(destination, NULL);
  hashtable_free(source, NULL);
}


static void
test_str_source_larger_than_capacity_for_empty_destination(void)
{
  copy_count = 0;
  free_count = 0;
  struct hashtable *source = alloc_and_fill_hashtable_with_strings(30, 16, 0, 1000);
  struct hashtable *destination = alloc_and_fill_hashtable_with_strings(15, 0, 0, 0);
  
  int result = hashtable_update(destination, source, copy_str_entry, free_str_entry);
  
  assert(-1 == result);
  assert(0 == destination->count);
  assert(0 == copy_count);
  assert(0 == free_count);
  
  free_count = 0;
  hashtable_free(destination, free_str_entry);
  assert(0 == free_count);

  free_count = 0;
  hashtable_free(source, free_str_entry);
  assert(16 == free_count);
}


static void
test_long_source_equals_capacity_for_empty_destination(void)
{
  struct hashtable *source = alloc_and_fill_hashtable_with_longs(30, 15, 100, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_longs(15, 0, 0, 0);
  
  int result = hashtable_update(destination, source, NULL, NULL);
  
  assert(0 == result);
  assert(15 == destination->count);
  assert_contains_long_values_for_long_keys_in_sequence(destination, 100, 100, 15);
  
  hashtable_free(destination, NULL);
  hashtable_free(source, NULL);
}


static void
test_str_source_equals_capacity_for_empty_destination(void)
{
  copy_count = 0;
  free_count = 0;
  
  struct hashtable *source = alloc_and_fill_hashtable_with_strings(30, 15, 100, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_strings(15, 0, 0, 0);
  
  int result = hashtable_update(destination, source, copy_str_entry, free_str_entry);
  
  assert(0 == result);
  assert(15 == destination->count);
  
  assert_contains_str_values_for_str_keys_in_sequence(destination, 100, 100, 15);
  assert(15 == copy_count);
  assert(0 == free_count);
  
  free_count = 0;
  hashtable_free(destination, free_str_entry);
  assert(15 == free_count);
  
  free_count = 0;
  hashtable_free(source, free_str_entry);
  assert(15 == free_count);
}


static void
test_long_source_larger_than_capacity(void)
{
  struct hashtable *source = alloc_and_fill_hashtable_with_longs(30, 16, 100, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_longs(15, 10, 0, 0);
  
  int result = hashtable_update(destination, source, NULL, NULL);
  
  assert(-1 == result);
  assert(10 == destination->count);
  
  hashtable_free(destination, NULL);
  hashtable_free(source, NULL);
}


static void
test_str_source_larger_than_capacity(void)
{
  copy_count = 0;
  free_count = 0;
  
  struct hashtable *source = alloc_and_fill_hashtable_with_strings(30, 16, 100, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_strings(15, 10, 0, 0);
  
  int result = hashtable_update(destination, source, copy_str_entry, free_str_entry);
  
  assert(-1 == result);
  assert(10 == destination->count);
  assert(0 == copy_count);
  assert(0 == free_count);
  
  free_count = 0;
  hashtable_free(destination, free_str_entry);
  assert(10 == free_count);
  
  free_count = 0;
  hashtable_free(source, free_str_entry);
  assert(16 == free_count);
}


static void
test_long_source_larger_than_available_no_overlap(void)
{
  struct hashtable *source = alloc_and_fill_hashtable_with_longs(15, 6, 100, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_longs(15, 10, 0, 0);
  
  int result = hashtable_update(destination, source, NULL, NULL);
  
  assert(-1 == result);
  assert(10 == destination->count);
  
  hashtable_free(destination, NULL);
  hashtable_free(source, NULL);
}


static void
test_str_source_larger_than_available_no_overlap(void)
{
  copy_count = 0;
  free_count = 0;
  
  struct hashtable *source = alloc_and_fill_hashtable_with_strings(15, 6, 100, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_strings(15, 10, 0, 0);
  
  int result = hashtable_update(destination, source, copy_str_entry, free_str_entry);
  
  assert(-1 == result);
  assert(10 == destination->count);
  assert(0 == copy_count);
  assert(0 == free_count);
  
  free_count = 0;
  hashtable_free(destination, free_str_entry);
  assert(10 == free_count);
  
  free_count = 0;
  hashtable_free(source, free_str_entry);
  assert(6 == free_count);
}


static void
test_long_source_larger_than_available_with_overlap(void)
{
  struct hashtable *source = alloc_and_fill_hashtable_with_longs(15, 7, 5, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_longs(15, 10, 0, 0);
  
  int result = hashtable_update(destination, source, NULL, NULL);
  
  assert(0 == result);
  assert(12 == destination->count);
  assert_contains_long_values_for_long_keys_in_sequence(destination, 0, 0, 5);
  assert_contains_long_values_for_long_keys_in_sequence(destination, 5, 100, 7);
  
  hashtable_free(destination, NULL);
  hashtable_free(source, NULL);
}


static void
test_string_source_larger_than_available_with_overlap(void)
{
  copy_count = 0;
  free_count = 0;
  
  struct hashtable *source = alloc_and_fill_hashtable_with_strings(15, 7, 5, 100);
  struct hashtable *destination = alloc_and_fill_hashtable_with_strings(15, 10, 0, 0);
  
  int result = hashtable_update(destination, source, copy_str_entry, free_str_entry);
  
  assert(0 == result);
  assert(12 == destination->count);
  assert(7 == copy_count);
  assert(5 == free_count);
  assert_contains_str_values_for_str_keys_in_sequence(destination, 0, 0, 5);
  assert_contains_str_values_for_str_keys_in_sequence(destination, 5, 100, 7);
  
  free_count = 0;
  hashtable_free(destination, free_str_entry);
  assert(12 == free_count);
  
  free_count = 0;
  hashtable_free(source, free_str_entry);
  assert(7 == free_count);
}


void
update_test(void)
{
  test_long_source_larger_than_capacity_for_empty_destination();
  test_str_source_larger_than_capacity_for_empty_destination();
  
  test_long_source_equals_capacity_for_empty_destination();
  test_str_source_equals_capacity_for_empty_destination();
  
  test_long_source_larger_than_capacity();
  test_str_source_larger_than_capacity();
  
  test_long_source_larger_than_available_no_overlap();
  test_str_source_larger_than_available_no_overlap();
  
  test_long_source_larger_than_available_with_overlap();
  test_string_source_larger_than_available_with_overlap();
}

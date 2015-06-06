#include "hashtable.h"


void
hashtable_clear(struct hashtable *hashtable, ht_free_entry *free_entry)
{
  int i = 0;
  while (hashtable->count) {
    if (hashtable->buckets[i].in_use) {
      if (free_entry) free_entry(hashtable, hashtable->buckets[i].entry);
      hashtable->buckets[i].in_use = false;
      --hashtable->count;
    }
    ++i;
  }
}


int
hashtable_copy_entries(struct hashtable const *hashtable,
                       struct ht_entry *entries,
                       int entries_count)
{
  int iterator = 0;
  int count = entries_count < hashtable->count
            ? entries_count
            : hashtable->count;
  for (int i = 0; i < count; ++i) {
    entries[i] = *hashtable_next(hashtable, &iterator);
  }
  return count;
}


int
hashtable_copy_keys(struct hashtable const *hashtable,
                    struct ht_key *keys,
                    int keys_count)
{
  int iterator = 0;
  int count = keys_count < hashtable->count ? keys_count : hashtable->count;
  for (int i = 0; i < count; ++i) {
    keys[i] = hashtable_next(hashtable, &iterator)->key;
  }
  return count;
}


int
hashtable_copy_values(struct hashtable const *hashtable,
                      union ht_value *values,
                      int values_count)
{
  int iterator = 0;
  int count = values_count < hashtable->count
            ? values_count
            : hashtable->count;
  for (int i = 0; i < count; ++i) {
    values[i] = hashtable_next(hashtable, &iterator)->value;
  }
  return count;
}


struct ht_entry const *
hashtable_get(struct hashtable const *hashtable, struct ht_key key)
{
  int index = key.hash % hashtable->capacity;
  
  for (int count = 0, i = index; count < hashtable->capacity; ++count, ++i) {
    if (i == hashtable->capacity) i = 0;
    
    if ( ! hashtable->buckets[i].in_use) break;
    
    if (hashtable->equal_keys(key, hashtable->buckets[i].entry.key)) {
      return &hashtable->buckets[i].entry;
    }
  }
  
  return NULL;
}


void
hashtable_init(struct hashtable *hashtable,
               int capacity,
               ht_equal_keys_func *equal_keys)
{
  hashtable->capacity = capacity;
  hashtable->count = 0;
  hashtable->equal_keys = equal_keys;
  hashtable->user_data = NULL;
  for (int i = 0; i < hashtable->capacity; ++i) {
    hashtable->buckets[i].in_use = false;
  }
}


struct ht_entry const *
hashtable_next(struct hashtable const *hashtable, int *iterator)
{
  while (*iterator < hashtable->capacity) {
    int i = (*iterator)++;
    if (hashtable->buckets[i].in_use) return &hashtable->buckets[i].entry;
  }
  return NULL;
}


int
hashtable_set(struct hashtable *hashtable,
              struct ht_key key,
              union ht_value value,
              bool *replaced,
              struct ht_entry *entry)
{
  int index = key.hash % hashtable->capacity;
  
  for (int count = 0, i = index; count < hashtable->capacity; ++count, ++i) {
    if (i == hashtable->capacity) i = 0;
    
    if ( ! hashtable->buckets[i].in_use) {
      hashtable->buckets[i].in_use = true;
      hashtable->buckets[i].entry.key = key;
      hashtable->buckets[i].entry.value = value;
      ++hashtable->count;
      if (replaced) *replaced = false;
      return 0;
    }
    
    if (hashtable->equal_keys(key, hashtable->buckets[i].entry.key)) {
      if (replaced) *replaced = true;
      if (entry) *entry = hashtable->buckets[i].entry;
      hashtable->buckets[i].entry.key = key;
      hashtable->buckets[i].entry.value = value;
      return 0;
    }
  }
  
  return -1;
}


int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 struct ht_entry *entry)
{
  int index = key.hash % hashtable->capacity;
  bool removed = false;
  
  for (int count = 0, i = index; count < hashtable->capacity; ++count, ++i) {
    if (i == hashtable->capacity) i = 0;
    
    if ( ! hashtable->buckets[i].in_use) break;
    
    if ( ! removed) {
      if (hashtable->equal_keys(key, hashtable->buckets[i].entry.key)) {
        if (entry) *entry = hashtable->buckets[i].entry;
        hashtable->buckets[i].in_use = false;
        --hashtable->count;
        removed = true;
      }
    } else {
      hashtable->buckets[i].in_use = false;
      --hashtable->count;
      hashtable_set(hashtable,
                    hashtable->buckets[i].entry.key,
                    hashtable->buckets[i].entry.value,
                    NULL,
                    NULL);
    }
  }
  
  return removed ? 0 : -1;
}


unsigned
ht_hash_of_const_str(char const *value)
{
  if ( ! value) return 0;
  
  unsigned hash = 0;
  while (*value) {
    hash = 31 * hash + *value;
    ++value;
  }
  return hash;
}


/* inline function extern declarations */

extern inline struct hashtable *
hashtable_alloc(int capacity, ht_equal_keys_func *equal_keys);

extern inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable);

extern inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable);

extern inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable);

extern inline void
hashtable_free(struct hashtable *hashtable, ht_free_entry *free_entry);

extern inline size_t
hashtable_size(int capacity);

extern inline struct ht_key
ht_const_str_key(char const *value);

extern inline union ht_value
ht_const_str_value(char const *value);

extern inline bool
ht_equal_const_str_keys(struct ht_key first, struct ht_key second);

extern inline bool
ht_equal_long_keys(struct ht_key first, struct ht_key second);

extern inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second);

extern inline bool
ht_equal_ulong_keys(struct ht_key first, struct ht_key second);

extern inline unsigned
ht_hash_of_long(long value);

extern inline unsigned
ht_hash_of_str(char *value);

extern inline unsigned
ht_hash_of_ulong(unsigned long value);

extern inline struct ht_key
ht_long_key(long value);

extern inline union ht_value
ht_long_value(long value);

extern inline struct ht_key
ht_str_key(char *value);

extern inline union ht_value
ht_str_value(char *value);

extern inline struct ht_key
ht_ulong_key(unsigned long value);

extern inline union ht_value
ht_ulong_value(unsigned long value);

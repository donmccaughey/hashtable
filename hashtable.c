#include "hashtable.h"


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
  
  for (int i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if ( ! hashtable->buckets[j].in_use) break;
    
    if (hashtable->equal_keys(key, hashtable->buckets[j].entry.key)) {
      return &hashtable->buckets[j].entry;
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
              bool *had_entry,
              struct ht_entry *entry_out)
{
  int index = key.hash % hashtable->capacity;
  
  for (int i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if ( ! hashtable->buckets[j].in_use) {
      hashtable->buckets[j].in_use = true;
      hashtable->buckets[j].entry.key = key;
      hashtable->buckets[j].entry.value = value;
      ++hashtable->count;
      if (had_entry) *had_entry = false;
      return 0;
    }
    
    if (hashtable->equal_keys(key, hashtable->buckets[j].entry.key)) {
      if (had_entry) *had_entry = true;
      if (entry_out) *entry_out = hashtable->buckets[j].entry;
      hashtable->buckets[j].entry.key = key;
      hashtable->buckets[j].entry.value = value;
      return 0;
    }
  }
  
  return -1;
}


int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 struct ht_entry *entry_out)
{
  int index = key.hash % hashtable->capacity;
  bool removed = false;
  
  for (int i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if ( ! hashtable->buckets[j].in_use) break;
    
    if ( ! removed) {
      if (hashtable->equal_keys(key, hashtable->buckets[j].entry.key)) {
        if (entry_out) *entry_out = hashtable->buckets[j].entry;
        hashtable->buckets[j].in_use = false;
        --hashtable->count;
        removed = true;
      }
    } else {
      hashtable->buckets[j].in_use = false;
      --hashtable->count;
      hashtable_set(hashtable,
                    hashtable->buckets[j].entry.key,
                    hashtable->buckets[j].entry.value,
                    NULL,
                    NULL);
    }
  }
  
  return removed ? 0 : -1;
}


extern inline struct hashtable *
hashtable_alloc(int capacity, ht_equal_keys_func *equal_keys);


extern inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable);


extern inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable);


extern inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable);


extern inline void
hashtable_free(struct hashtable *hashtable);


extern inline size_t
hashtable_size(int capacity);


extern inline struct ht_key
ht_alloc_str_key(char const *value);


extern inline union ht_value
ht_alloc_str_value(char const *value);


extern inline bool
ht_equal_const_str_keys(struct ht_key first, struct ht_key second);


extern inline bool
ht_equal_const_str_values(union ht_value first, union ht_value second);


extern inline bool
ht_equal_int_keys(struct ht_key first, struct ht_key second);


extern inline bool
ht_equal_int_values(union ht_value first, union ht_value second);


extern inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second);


extern inline bool
ht_equal_str_values(union ht_value first, union ht_value second);


extern inline bool
ht_equal_uint_keys(struct ht_key first, struct ht_key second);


extern inline bool
ht_equal_uint_values(union ht_value first, union ht_value second);


extern inline void
ht_free_str_key(struct ht_key key);


extern inline void
ht_free_str_value(union ht_value value);


uint32_t
ht_hash_of_const_str(char const *value)
{
  if ( ! value) return 0;
  
  uint32_t hash = 0;
  while (*value) {
    hash = 31 * hash + *value;
    ++value;
  }
  return hash;
}


extern inline uint32_t
ht_hash_of_int(long value);


extern inline uint32_t
ht_hash_of_str(char *value);


extern inline uint32_t
ht_hash_of_uint(unsigned long value);


extern inline struct ht_key
ht_int_key(long value);


extern inline union ht_value
ht_int_value(long value);


extern inline struct ht_key
ht_uint_key(unsigned long value);


extern inline union ht_value
ht_uint_value(unsigned long value);

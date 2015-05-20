#include "hashtable.h"


size_t
hashtable_copy_entries(struct hashtable const *hashtable,
                       struct ht_entry *entries,
                       size_t entries_count)
{
  size_t iterator = 0;
  size_t count = entries_count < hashtable->count
               ? entries_count
               : hashtable->count;
  for (size_t i = 0; i < count; ++i) {
    entries[i] = *hashtable_next(hashtable, &iterator);
  }
  return count;
}


size_t
hashtable_copy_keys(struct hashtable const *hashtable,
                    struct ht_key *keys,
                    size_t keys_count)
{
  size_t iterator = 0;
  size_t count = keys_count < hashtable->count ? keys_count : hashtable->count;
  for (size_t i = 0; i < count; ++i) {
    keys[i] = hashtable_next(hashtable, &iterator)->key;
  }
  return count;
}


size_t
hashtable_copy_values(struct hashtable const *hashtable,
                      union ht_value *values,
                      size_t values_count)
{
  size_t iterator = 0;
  size_t count = values_count < hashtable->count
               ? values_count
               : hashtable->count;
  for (size_t i = 0; i < count; ++i) {
    values[i] = hashtable_next(hashtable, &iterator)->value;
  }
  return count;
}


struct ht_entry const *
hashtable_get(struct hashtable const *hashtable, struct ht_key key)
{
  size_t index = key.hash % hashtable->capacity;
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
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
               size_t capacity,
               ht_equal_keys_func *equal_keys)
{
  hashtable->capacity = capacity;
  hashtable->count = 0;
  hashtable->equal_keys = equal_keys;
  hashtable->user_data = NULL;
  for (size_t i = 0; i < hashtable->capacity; ++i) {
    hashtable->buckets[i].in_use = false;
  }
}


struct ht_entry const *
hashtable_next(struct hashtable const *hashtable, size_t *iterator)
{
  while (*iterator < hashtable->capacity) {
    size_t i = (*iterator)++;
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
  size_t index = key.hash % hashtable->capacity;
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
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
  size_t index = key.hash % hashtable->capacity;
  bool removed = false;
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
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
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys);


extern inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable);


extern inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable);


extern inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable);


extern inline void
hashtable_free(struct hashtable *hashtable);


extern inline size_t
hashtable_size(size_t capacity);


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


size_t
ht_hash_of_const_str(char const *value)
{
  if ( ! value) return 0;
  
  size_t hash = 0;
  while (*value) {
    hash = 31 * hash + *value;
    ++value;
  }
  return hash;
}


extern inline size_t
ht_hash_of_int(ht_int_t value);


extern inline size_t
ht_hash_of_str(char *value);


extern inline size_t
ht_hash_of_uint(ht_uint_t value);


extern inline struct ht_key
ht_int_key(ht_int_t value);


extern inline union ht_value
ht_int_value(ht_int_t value);


extern inline struct ht_key
ht_uint_key(ht_uint_t value);


extern inline union ht_value
ht_uint_value(ht_uint_t value);

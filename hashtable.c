#include "hashtable.h"


struct hashtable *
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys)
{
  size_t size = sizeof(struct hashtable) + sizeof(struct ht_bucket[capacity]);
  struct hashtable *hashtable = calloc(1, size);
  if ( ! hashtable) return NULL;
  
  hashtable->capacity = capacity;
  hashtable->equal_keys = equal_keys;
  
  return hashtable;
}


struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable)
{
  struct ht_key *keys = calloc(hashtable->count, sizeof(struct ht_key));
  if ( ! keys) return NULL;
  
  size_t iterator = 0;
  size_t i = 0;
  while (hashtable_next(hashtable, &iterator, &keys[i++], NULL))
    ;
  
  return keys;
}


union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable)
{
  union ht_value *values = calloc(hashtable->count, sizeof(union ht_value));
  if ( ! values) return NULL;
  
  size_t iterator = 0;
  size_t i = 0;
  while (hashtable_next(hashtable, &iterator, NULL, &values[i++]))
    ;
  
  return values;
}


struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable)
{
  struct ht_entry *entries = calloc(hashtable->count, sizeof(struct ht_entry));
  if ( ! entries) return NULL;
  
  size_t iterator = 0;
  size_t i = 0;
  while (hashtable_next(hashtable, &iterator, &entries[i].key, &entries[i].value)) {
    ++i;
  }
  
  return entries;
}


void
hashtable_free(struct hashtable *hashtable)
{
  free(hashtable);
}


int
hashtable_get(struct hashtable const *hashtable,
              struct ht_key key,
              struct ht_entry *entry_out)
{
  size_t index = key.hash % hashtable->capacity;
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if ( ! hashtable->buckets[j].in_use) break;
    
    if (hashtable->equal_keys(key, hashtable->buckets[j].entry.key)) {
      if (entry_out) *entry_out = hashtable->buckets[j].entry;
      return 0;
    }
  }
  
  return -1;
}


bool
hashtable_next(struct hashtable const *hashtable,
               size_t *iterator,
               struct ht_key *key_out,
               union ht_value *value_out)
{
  while (*iterator < hashtable->capacity) {
    size_t i = (*iterator)++;
    if (hashtable->buckets[i].in_use) {
      if (key_out) *key_out = hashtable->buckets[i].entry.key;
      if (value_out) *value_out = hashtable->buckets[i].entry.value;
      return true;
    }
  }
  return false;
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

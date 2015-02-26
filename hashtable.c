#include "hashtable.h"


struct ht_entry {
  struct ht_entry *next;
  struct ht_key key;
  union ht_value value;
};


struct hashtable {
  struct ht_entry **entries;
  size_t capacity;
  size_t count;
  ht_equal_keys_func *equal_keys;
};


static struct ht_entry *
alloc_entry(struct ht_key key, union ht_value value);

static size_t
get_index(struct hashtable const *hashtable, struct ht_key key);

extern inline struct ht_key
ht_alloc_str_key(char const *value);

extern inline union ht_value
ht_alloc_str_value(char const *value);

extern inline void
ht_free_str_key(struct ht_key key);

extern inline void
ht_free_str_value(union ht_value value);

extern inline size_t
ht_hash_of_str(char *value);

extern inline size_t
ht_hash_of_int(ht_int_t value);

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


static struct ht_entry *
alloc_entry(struct ht_key key, union ht_value value)
{
  struct ht_entry *entry = calloc(1, sizeof(struct ht_entry));
  if ( ! entry) return NULL;
  entry->key = key;
  entry->value = value;
  return entry;
}


static size_t
get_index(struct hashtable const *hashtable, struct ht_key key)
{
  return key.hash % hashtable->capacity;
}


struct hashtable *
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys)
{
  struct hashtable *hashtable = calloc(1, sizeof(struct hashtable));
  if ( ! hashtable) return NULL;
  
  hashtable->entries = calloc(capacity, sizeof(struct ht_entry *));
  if ( ! hashtable->entries) {
    free(hashtable);
    return NULL;
  }
  
  hashtable->capacity = capacity;
  hashtable->equal_keys = equal_keys;
  
  return hashtable;
}


struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable)
{
  struct ht_key *keys = calloc(hashtable->count, sizeof(struct ht_key));
  if ( ! keys) return NULL;
  
  size_t i = 0;
  for (size_t j = 0; j < hashtable->capacity; ++j) {
    if (hashtable->entries[j]) {
      struct ht_entry *entry = hashtable->entries[j];
      while (entry) {
        keys[i] = entry->key;
        ++i;
        entry = entry->next;
      }
    }
  }
  
  return keys;
}


union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable)
{
  union ht_value *values = calloc(hashtable->count, sizeof(union ht_value));
  if ( ! values) return NULL;
  
  size_t i = 0;
  for (size_t j = 0; j < hashtable->capacity; ++j) {
    if (hashtable->entries[j]) {
      struct ht_entry *entry = hashtable->entries[j];
      while (entry) {
        values[i] = entry->value;
        ++i;
        entry = entry->next;
      }
    }
  }
  
  return values;
}


size_t
hashtable_capacity(struct hashtable const *hashtable)
{
  return hashtable->capacity;
}


size_t
hashtable_count(struct hashtable const *hashtable)
{
  return hashtable->count;
}


void
hashtable_free(struct hashtable *hashtable)
{
  free(hashtable->entries);
  free(hashtable);
}


int
hashtable_get(struct hashtable const *hashtable,
              struct ht_key key,
              union ht_value *value_out)
{
  size_t index = get_index(hashtable, key);
  
  if (hashtable->entries[index]) {
    struct ht_entry *entry = hashtable->entries[index];
    do {
      if (hashtable->equal_keys(key, entry->key)) {
        if (value_out) *value_out = entry->value;
        return 0;
      }
      entry = entry->next;
    } while (entry);
  }
  
  return -1;
}


int
hashtable_put(struct hashtable *hashtable,
              struct ht_key key,
              union ht_value value,
              union ht_value *previous_value_out)
{
  size_t index = get_index(hashtable, key);
  
  if (hashtable->entries[index]) {
    struct ht_entry *entry = hashtable->entries[index];
    do {
      if (hashtable->equal_keys(key, entry->key)) {
        if (previous_value_out) *previous_value_out = entry->value;
        entry->value = value;
        return 0;
      }
      entry = entry->next;
    } while (entry);
  }
  
  struct ht_entry *entry = alloc_entry(key, value);
  if ( ! entry) return -1;
  entry->next = hashtable->entries[index];
  hashtable->entries[index] = entry;
  ++hashtable->count;
  
  return 0;
}


int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 union ht_value *previous_value_out)
{
  size_t index = get_index(hashtable, key);
  
  if (hashtable->entries[index]) {
    struct ht_entry *entry = hashtable->entries[index];
    struct ht_entry *previous_entry = NULL;
    do {
      if (hashtable->equal_keys(key, entry->key)) {
        if (previous_entry) {
          previous_entry->next = entry->next;
        } else {
          hashtable->entries[index] = NULL;
        }
        if (previous_value_out) *previous_value_out = entry->value;
        free(entry);
        --hashtable->count;
        return 0;
      }
      previous_entry = entry;
      entry = entry->next;
    } while (entry);
  }
  
  return -1;
}


bool
ht_equal_const_str_keys(struct ht_key first_key, struct ht_key second_key)
{
  return 0 == strcmp(first_key.value.const_str_value, second_key.value.const_str_value);
}


bool
ht_equal_int_keys(struct ht_key first_key, struct ht_key second_key)
{
  return first_key.value.int_value == second_key.value.int_value;
}


bool
ht_equal_str_keys(struct ht_key first_key, struct ht_key second_key)
{
  return 0 == strcmp(first_key.value.str_value, second_key.value.str_value);
}


bool
ht_equal_uint_keys(struct ht_key first_key, struct ht_key second_key)
{
  return first_key.value.uint_value == second_key.value.uint_value;
}


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

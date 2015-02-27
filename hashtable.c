#include "hashtable.h"


#define ENTRY_IN_USE  0x01
#define ENTRY_REMOVED 0x02


struct ht_entry {
  unsigned flags;
  struct ht_key key;
  union ht_value value;
};


struct hashtable {
  size_t capacity;
  size_t count;
  ht_equal_keys_func *equal_keys;
  struct ht_entry entries[];
};


static inline size_t
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


static inline size_t
get_index(struct hashtable const *hashtable, struct ht_key key)
{
  return key.hash % hashtable->capacity;
}


struct hashtable *
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys)
{
  size_t size = sizeof(struct hashtable) + sizeof(struct ht_entry[capacity]);
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
  
  for (size_t i = 0, j = 0; i < hashtable->capacity; ++i) {
    if (ENTRY_IN_USE & hashtable->entries[i].flags) {
      keys[j++] = hashtable->entries[i].key;
    }
  }
  
  return keys;
}


union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable)
{
  union ht_value *values = calloc(hashtable->count, sizeof(union ht_value));
  if ( ! values) return NULL;
  
  for (size_t i = 0, j = 0; i < hashtable->capacity; ++i) {
    if (ENTRY_IN_USE & hashtable->entries[i].flags) {
      values[j++] = hashtable->entries[i].value;
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
  free(hashtable);
}


int
hashtable_get(struct hashtable const *hashtable,
              struct ht_key key,
              union ht_value *value_out)
{
  size_t index = get_index(hashtable, key);
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if ( ! (ENTRY_IN_USE | ENTRY_REMOVED) & hashtable->entries[j].flags) return -1;
    
    if (   ENTRY_IN_USE & hashtable->entries[j].flags
        && hashtable->equal_keys(key, hashtable->entries[j].key))
    {
      if (value_out) *value_out = hashtable->entries[j].value;
      return 0;
    }
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
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if (ENTRY_IN_USE & hashtable->entries[j].flags) {
      if (hashtable->equal_keys(key, hashtable->entries[j].key)) {
        if (previous_value_out) *previous_value_out = hashtable->entries[j].value;
        hashtable->entries[j].value = value;
        return 0;
      }
    } else {
      hashtable->entries[j].flags = ENTRY_IN_USE;
      hashtable->entries[j].key = key;
      hashtable->entries[j].value = value;
      ++hashtable->count;
      return 0;
    }
  }
  
  return -1;
}


int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 union ht_value *previous_value_out)
{
  size_t index = get_index(hashtable, key);
  
  for (size_t i = 0, j = index; i < hashtable->capacity; ++i, ++j) {
    if (j == hashtable->capacity) j = 0;
    
    if ( ! (ENTRY_IN_USE | ENTRY_REMOVED) & hashtable->entries[j].flags) return -1;
    
    if (   ENTRY_IN_USE & hashtable->entries[j].flags
        && hashtable->equal_keys(key, hashtable->entries[j].key))
    {
      if (previous_value_out) *previous_value_out = hashtable->entries[j].value;
      hashtable->entries[j].flags = ENTRY_REMOVED;
      --hashtable->count;
      return 0;
    }
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

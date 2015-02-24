#include "hash_table.h"


struct ht_entry {
  struct ht_entry *next;
  struct ht_key key;
  union ht_value value;
};


struct hash_table {
  struct ht_entry **entries;
  size_t capacity;
  size_t count;
  ht_equal_keys_func *equal_keys;
};


static struct ht_entry *
alloc_entry(struct ht_key key, union ht_value value);

static size_t
get_index(struct hash_table const *hash_table, struct ht_key key);

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
get_index(struct hash_table const *hash_table, struct ht_key key)
{
  return key.hash % hash_table->capacity;
}


struct hash_table *
hash_table_alloc(size_t capacity, ht_equal_keys_func *equal_keys)
{
  struct hash_table *hash_table = calloc(1, sizeof(struct hash_table));
  if ( ! hash_table) return NULL;
  
  hash_table->entries = calloc(capacity, sizeof(struct ht_entry *));
  if ( ! hash_table->entries) {
    free(hash_table);
    return NULL;
  }
  
  hash_table->capacity = capacity;
  hash_table->equal_keys = equal_keys;
  
  return hash_table;
}


struct ht_key *
hash_table_alloc_keys(struct hash_table const *hash_table)
{
  struct ht_key *keys = calloc(hash_table->count, sizeof(struct ht_key));
  if ( ! keys) return NULL;
  
  size_t i = 0;
  for (size_t j = 0; j < hash_table->capacity; ++j) {
    if (hash_table->entries[j]) {
      struct ht_entry *entry = hash_table->entries[j];
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
hash_table_alloc_values(struct hash_table const *hash_table)
{
  union ht_value *values = calloc(hash_table->count, sizeof(union ht_value));
  if ( ! values) return NULL;
  
  size_t i = 0;
  for (size_t j = 0; j < hash_table->capacity; ++j) {
    if (hash_table->entries[j]) {
      struct ht_entry *entry = hash_table->entries[j];
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
hash_table_capacity(struct hash_table const *hash_table)
{
  return hash_table->capacity;
}


size_t
hash_table_count(struct hash_table const *hash_table)
{
  return hash_table->count;
}


void
hash_table_free(struct hash_table *hash_table)
{
  free(hash_table->entries);
  free(hash_table);
}


int
hash_table_get(struct hash_table const *hash_table,
               struct ht_key key,
               union ht_value *value_out)
{
  size_t index = get_index(hash_table, key);
  
  if (hash_table->entries[index]) {
    struct ht_entry *entry = hash_table->entries[index];
    do {
      if (hash_table->equal_keys(key, entry->key)) {
        if (value_out) *value_out = entry->value;
        return 0;
      }
      entry = entry->next;
    } while (entry);
  }
  
  return -1;
}


int
hash_table_put(struct hash_table *hash_table,
               struct ht_key key,
               union ht_value value,
               union ht_value *previous_value_out)
{
  size_t index = get_index(hash_table, key);
  
  if (hash_table->entries[index]) {
    struct ht_entry *entry = hash_table->entries[index];
    do {
      if (hash_table->equal_keys(key, entry->key)) {
        if (previous_value_out) *previous_value_out = entry->value;
        entry->value = value;
        return 0;
      }
      entry = entry->next;
    } while (entry);
  }
  
  struct ht_entry *entry = alloc_entry(key, value);
  if ( ! entry) return -1;
  entry->next = hash_table->entries[index];
  hash_table->entries[index] = entry;
  ++hash_table->count;
  
  return 0;
}


int
hash_table_remove(struct hash_table *hash_table,
                  struct ht_key key,
                  union ht_value *previous_value_out)
{
  size_t index = get_index(hash_table, key);
  
  if (hash_table->entries[index]) {
    struct ht_entry *entry = hash_table->entries[index];
    struct ht_entry *previous_entry = NULL;
    do {
      if (hash_table->equal_keys(key, entry->key)) {
        if (previous_entry) {
          previous_entry->next = entry->next;
        } else {
          hash_table->entries[index] = NULL;
        }
        if (previous_value_out) *previous_value_out = entry->value;
        free(entry);
        --hash_table->count;
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

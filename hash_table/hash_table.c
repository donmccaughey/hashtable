#include "hash_table.h"

#include <stdlib.h>


struct ht_entry {
  struct ht_entry *next;
  struct ht_key key;
  union ht_value value;
};


struct hash_table {
  struct ht_entry **entries;
  size_t capacity;
  size_t count;
  ht_equals_function *equals;
};


static struct ht_entry *
alloc_entry(struct ht_key key, union ht_value value);

static size_t
get_index(struct hash_table const *hash_table, struct ht_key key);


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
hash_table_alloc(size_t capacity, ht_equals_function *equals)
{
  struct hash_table *hash_table = calloc(1, sizeof(struct hash_table));
  if ( ! hash_table) return NULL;
  
  hash_table->entries = calloc(capacity, sizeof(struct ht_entry *));
  if ( ! hash_table->entries) {
    free(hash_table);
    return NULL;
  }
  
  hash_table->capacity = capacity;
  hash_table->equals = equals;
  
  return hash_table;
}


struct ht_key *
hash_table_alloc_keys(struct hash_table const *hash_table)
{
  struct ht_key *keys = calloc(hash_table->count, sizeof(struct ht_key));
  if ( ! keys) return NULL;
  
  size_t key_index = 0;
  for (size_t entry_index = 0; entry_index < hash_table->capacity; ++entry_index) {
    if (hash_table->entries[entry_index]) {
      struct ht_entry *entry = hash_table->entries[entry_index];
      while (entry) {
        keys[key_index] = entry->key;
        ++key_index;
        entry = entry->next;
      }
    }
  }
  
  return keys;
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
      if (hash_table->equals(key, entry->key)) {
        if (value_out) *value_out = entry->value;
        return 0;
      }
      entry = entry->next;
    } while (entry);
  }
  
  return -1;
}


bool
hash_table_has_key(struct hash_table const *hash_table, struct ht_key key)
{
  return 0 == hash_table_get(hash_table, key, NULL);
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
      if (hash_table->equals(key, entry->key)) {
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
      if (hash_table->equals(key, entry->key)) {
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

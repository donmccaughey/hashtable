#include "hash_table.h"

#include <stdlib.h>


struct hash_table_entry {
  struct hash_table_entry *next;
  void const *key;
  void const *value;
};


struct hash_table {
  struct hash_table_entry **entries;
  uint32_t capacity;
  uint32_t count;
  hash_table_hash_function *hash;
  hash_table_equals_function *equals;
};


static struct hash_table_entry *
hash_table_entry_alloc(void const *key, void const *value);


struct hash_table *
hash_table_alloc(uint32_t capacity,
                 hash_table_hash_function *hash,
                 hash_table_equals_function *equals)
{
  struct hash_table *hash_table = calloc(1, sizeof(struct hash_table));
  if ( ! hash_table) return NULL;
  
  hash_table->entries = calloc(capacity, sizeof(struct hash_table_entry *));
  if ( ! hash_table->entries) {
    free(hash_table);
    return NULL;
  }
  
  hash_table->capacity = capacity;
  hash_table->hash = hash;
  hash_table->equals = equals;
  
  return hash_table;
}


void const **
hash_table_alloc_keys(struct hash_table const *hash_table)
{
  void const **keys = calloc(hash_table->count, sizeof(void *));
  if ( ! keys) return NULL;
  
  size_t key_index = 0;
  for (size_t entry_index = 0; entry_index < hash_table->capacity; ++entry_index) {
    if (hash_table->entries[entry_index]) {
      struct hash_table_entry *entry = hash_table->entries[entry_index];
      while (entry) {
        keys[key_index] = entry->key;
        ++key_index;
        entry = entry->next;
      }
    }
  }
  
  return keys;
}


uint32_t
hash_table_capacity(struct hash_table const *hash_table)
{
  return hash_table->capacity;
}


uint32_t
hash_table_count(struct hash_table const *hash_table)
{
  return hash_table->count;
}


static struct hash_table_entry *
hash_table_entry_alloc(void const *key, void const *value)
{
  struct hash_table_entry *entry = calloc(1, sizeof(struct hash_table_entry));
  if ( ! entry) return NULL;
  entry->key = key;
  entry->value = value;
  return entry;
}


void
hash_table_free(struct hash_table *hash_table)
{
  free(hash_table->entries);
  free(hash_table);
}


void const *
hash_table_get(struct hash_table const *hash_table, void const *key)
{
  uint32_t hash = hash_table->hash(key);
  size_t index = hash % hash_table->capacity;
  
  if (hash_table->entries[index]) {
    struct hash_table_entry *entry = hash_table->entries[index];
    do {
      if (hash_table->equals(key, entry->key)) return entry->value;
      entry = entry->next;
    } while (entry);
  }
  
  return NULL;
}


bool
hash_table_has_key(struct hash_table const *hash_table, void const *key)
{
  uint32_t hash = hash_table->hash(key);
  size_t index = hash % hash_table->capacity;
  
  if (hash_table->entries[index]) {
    struct hash_table_entry *entry = hash_table->entries[index];
    do {
      if (hash_table->equals(key, entry->key)) return true;
      entry = entry->next;
    } while (entry);
  }
  
  return false;
}


int
hash_table_put(struct hash_table *hash_table,
               void const *key,
               void const *value,
               void const **previous_value)
{
  uint32_t hash = hash_table->hash(key);
  size_t index = hash % hash_table->capacity;
  
  if (hash_table->entries[index]) {
    struct hash_table_entry *entry = hash_table->entries[index];
    do {
      if (hash_table->equals(key, entry->key)) {
        if (previous_value) *previous_value = entry->value;
        entry->value = value;
        return 0;
      }
      entry = entry->next;
    } while (entry);
  }
  
  if (previous_value) *previous_value = NULL;
  struct hash_table_entry *entry = hash_table_entry_alloc(key, value);
  if ( ! entry) return -1;
  entry->next = hash_table->entries[index];
  hash_table->entries[index] = entry;
  ++hash_table->count;
  
  return 0;
}


void const *
hash_table_remove(struct hash_table *hash_table, void const *key)
{
  uint32_t hash = hash_table->hash(key);
  size_t index = hash % hash_table->capacity;
  
  if (hash_table->entries[index]) {
    struct hash_table_entry *entry = hash_table->entries[index];
    struct hash_table_entry *previous_entry = NULL;
    do {
      if (hash_table->equals(key, entry->key)) {
        if (previous_entry) {
          previous_entry->next = entry->next;
        } else {
          hash_table->entries[index] = NULL;
        }
        void const *value = entry->value;
        free(entry);
        --hash_table->count;
        return value;
      }
      previous_entry = entry;
      entry = entry->next;
    } while (entry);
  }
  
  return NULL;
}

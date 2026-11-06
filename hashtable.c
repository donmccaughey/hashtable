#include "hashtable.h"


void
hashtable_clear(struct hashtable *hashtable, ht_free_entry *free_entry)
{
  size_t i = 0;
  while (hashtable->count) {
    if (hashtable->buckets[i].in_use) {
      if (free_entry) free_entry(hashtable, hashtable->buckets[i].entry);
      hashtable->buckets[i].in_use = false;
      --hashtable->count;
    }
    ++i;
  }
}


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

  for (size_t count = 0, i = index; count < hashtable->capacity; ++count, ++i) {
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
              bool *replaced,
              struct ht_entry *entry)
{
  size_t index = key.hash % hashtable->capacity;

  for (size_t count = 0, i = index; count < hashtable->capacity; ++count, ++i) {
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
  size_t index = key.hash % hashtable->capacity;
  bool removed = false;
  
  for (size_t count = 0, i = index; count < hashtable->capacity; ++count, ++i) {
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


int
hashtable_update(struct hashtable *destination,
                 struct hashtable const *source,
                 ht_copy_entry copy_entry,
                 ht_free_entry *free_entry)
{
  if (source->count > destination->capacity) return -1;
  
  size_t available = destination->capacity - destination->count;
  if (source->count > available) {
    size_t iterator = 0;
    for (size_t i = 0; i < source->count; ++i) {
      struct ht_entry const *source_entry = hashtable_next(source, &iterator);
      if (hashtable_get(destination, source_entry->key)) ++available;
      if (source->count == available) break;
    }
  }
  if (source->count > available) return -1;
  
  size_t iterator = 0;
  for (size_t i = 0; i < source->count; ++i) {
    struct ht_entry const *source_entry = hashtable_next(source, &iterator);
    struct ht_entry destination_entry;
    if (copy_entry) {
      int result = copy_entry(destination, &destination_entry, *source_entry);
      if (-1 == result) return -1;
    } else {
      destination_entry = *source_entry;
    }
    bool replaced;
    struct ht_entry entry;
    int result = hashtable_set(destination,
                               destination_entry.key,
                               destination_entry.value,
                               &replaced,
                               &entry);
    if (-1 == result) return -1;
    if (replaced && free_entry) free_entry(destination, entry);
  }
  return 0;
}


ht_hash
ht_fnv1a_init(void)
{
#if HASHTABLE_BITS == 64
  return 0xCBF29CE484222325ULL;
#else
  return 0x811c9dc5u;
#endif
}


ht_hash
ht_fnv1a_append(ht_hash hash, void const *start, size_t size)
{
  unsigned char const *octets = start;
  for (size_t i = 0; i < size; ++i) {
    hash ^= (ht_hash)octets[i];
#if HASHTABLE_BITS == 64
    hash *= 0x00000100000001B3ULL;
#else
    hash *= 0x01000193u;
#endif
  }
  return hash;
}


/* inline function extern declarations */

extern inline struct hashtable *
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys);

extern inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable);

extern inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable);

extern inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable);

extern inline void
hashtable_free(struct hashtable *hashtable, ht_free_entry *free_entry);

extern inline size_t
hashtable_size(size_t capacity);

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

extern inline ht_hash
ht_hash_of(void const *hash_data, size_t hash_data_size);

extern inline ht_hash
ht_hash_of_const_str(char const *value);

extern inline ht_hash
ht_hash_of_long(long value);

extern inline ht_hash
ht_hash_of_str(char *value);

extern inline ht_hash
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

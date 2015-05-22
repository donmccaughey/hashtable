#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED


#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/* A value contains a pointer type or a long integer type. */
union ht_value {
  void const *const_ptr_value;
  char const *const_str_value;
  long long_value;
  void *ptr_value;
  char *str_value;
  unsigned long ulong_value;
};


/* A key contains a value and the computed hash of that value. */
struct ht_key {
  unsigned hash;
  union ht_value value;
};


/* An entry contains an key and corresponding value. */
struct ht_entry {
  struct ht_key key;
  union ht_value value;
};


/* A space for an entry in the hash table, which may be used or unused. */
struct ht_bucket {
  bool in_use;
  struct ht_entry entry;
};


/* Equality function type for keys. */
typedef bool
ht_equal_keys_func(struct ht_key first, struct ht_key second);


/* The hash table. */
struct hashtable {
  int capacity;
  int count;
  ht_equal_keys_func *equal_keys;
  void *user_data;
  struct ht_bucket buckets[];
};


/* Initialize a hash table. */

void
hashtable_init(struct hashtable *hashtable,
               int capacity,
               ht_equal_keys_func *equal_keys);

inline size_t
hashtable_size(int capacity)
{
  return sizeof(struct hashtable) + sizeof(struct ht_bucket[capacity]);
}


/* Allocate and free a hash table. */

inline struct hashtable *
hashtable_alloc(int capacity, ht_equal_keys_func *equal_keys)
{
  struct hashtable *hashtable = malloc(hashtable_size(capacity));
  if (hashtable) hashtable_init(hashtable, capacity, equal_keys);
  return hashtable;
}

inline void
hashtable_free(struct hashtable *hashtable)
{
  free(hashtable);
}


/* Read, write and delete entries. */

struct ht_entry const *
hashtable_get(struct hashtable const *hashtable, struct ht_key key);

int
hashtable_set(struct hashtable *hashtable,
              struct ht_key key,
              union ht_value value,
              bool *replaced,
              struct ht_entry *entry);

int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 struct ht_entry *entry);


/* Iterate over keys and values. */

struct ht_entry const *
hashtable_next(struct hashtable const *hashtable, int *iterator);

int
hashtable_copy_keys(struct hashtable const *hashtable,
                    struct ht_key *keys,
                    int keys_count);

int
hashtable_copy_values(struct hashtable const *hashtable,
                      union ht_value *values,
                      int values_count);


int
hashtable_copy_entries(struct hashtable const *hashtable,
                       struct ht_entry *entries,
                       int entries_count);

inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable)
{
  struct ht_key *keys = malloc(hashtable->count * sizeof(struct ht_key));
  if (keys) hashtable_copy_keys(hashtable, keys, hashtable->count);
  return keys;
}

inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable)
{
  union ht_value *values = malloc(hashtable->count * sizeof(union ht_value));
  if (values) hashtable_copy_values(hashtable, values, hashtable->count);
  return values;
}

inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable)
{
  struct ht_entry *entries = malloc(hashtable->count * sizeof(struct ht_entry));
  if (entries) hashtable_copy_entries(hashtable, entries, hashtable->count);
  return entries;
}


/* Make values. */

inline union ht_value
ht_const_str_value(char const *value)
{
  return (union ht_value){ .const_str_value = value };
}

inline union ht_value
ht_long_value(long value)
{
  return (union ht_value){ .long_value = value };
}

inline union ht_value
ht_str_value(char *value)
{
  return (union ht_value){ .str_value = value };
}

inline union ht_value
ht_ulong_value(unsigned long value)
{
  return (union ht_value){ .ulong_value = value };
}


/* Hash functions. */

inline unsigned
ht_hash_of_ulong(unsigned long value)
{
  return value % UINT_MAX;
}

inline unsigned
ht_hash_of_long(long value)
{
  return ht_hash_of_ulong((unsigned long)value);
}

unsigned
ht_hash_of_const_str(char const *value);

inline unsigned
ht_hash_of_str(char *value)
{
  return ht_hash_of_const_str(value);
}


/* Make keys. */

inline struct ht_key
ht_const_str_key(char const *value)
{
  return (struct ht_key){
    .hash = ht_hash_of_const_str(value),
    .value = ht_const_str_value(value)
  };
}

inline struct ht_key
ht_long_key(long value)
{
  return (struct ht_key){
    .hash = ht_hash_of_long(value),
    .value = ht_long_value(value)
  };
}

inline struct ht_key
ht_str_key(char *value)
{
  return (struct ht_key){
    .hash = ht_hash_of_str(value),
    .value = ht_str_value(value)
  };
}

inline struct ht_key
ht_ulong_key(unsigned long value)
{
  return (struct ht_key){
    .hash = ht_hash_of_ulong(value),
    .value = ht_ulong_value(value)
  };
}


/* Equality functions for keys. */

inline bool
ht_equal_const_str_keys(struct ht_key first, struct ht_key second)
{
  return first.hash == second.hash
      && 0 == strcmp(first.value.const_str_value, second.value.const_str_value);
}

inline bool
ht_equal_long_keys(struct ht_key first, struct ht_key second)
{
  return first.value.long_value == second.value.long_value;
}

inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second)
{
  return first.hash == second.hash
      && 0 == strcmp(first.value.str_value, second.value.str_value);
}

inline bool
ht_equal_ulong_keys(struct ht_key first, struct ht_key second)
{
  return first.value.ulong_value == second.value.ulong_value;
}


#endif

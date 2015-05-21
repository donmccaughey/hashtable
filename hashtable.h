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


/* An entry in the hash table. */
struct ht_entry {
  struct ht_key key;
  union ht_value value;
};


/* A space for an entry in the hash table, which may be used or unused. */
struct ht_bucket {
  bool in_use;
  struct ht_entry entry;
};


/* Function types for comparing keys and values. */

typedef bool
ht_equal_keys_func(struct ht_key first, struct ht_key second);

typedef bool
ht_equal_values_func(union ht_value first, union ht_value second);


/* The hash table. */
struct hashtable {
  int capacity;
  int count;
  ht_equal_keys_func *equal_keys;
  void *user_data;
  struct ht_bucket buckets[];
};


/* Hash table creation and destruction functions. */

inline struct hashtable *
hashtable_alloc(int capacity, ht_equal_keys_func *equal_keys);

inline void
hashtable_free(struct hashtable *hashtable);

void
hashtable_init(struct hashtable *hashtable,
               int capacity,
               ht_equal_keys_func *equal_keys);

inline size_t
hashtable_size(int capacity);


/* Functions to read, write and delete entries in the hash table. */

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


/* Functions to iterate over keys and values in the hash table. */

struct ht_entry const *
hashtable_next(struct hashtable const *hashtable, int *iterator);

inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable);

inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable);

inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable);

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


/* Functions to make keys. */

inline struct ht_key
ht_const_str_key(char const *value);

inline struct ht_key
ht_long_key(long value);

inline struct ht_key
ht_str_key(char *value);

inline struct ht_key
ht_ulong_key(unsigned long value);


/* Functions to make values. */

inline union ht_value
ht_const_str_value(char const *value);

inline union ht_value
ht_long_value(long value);

inline union ht_value
ht_str_value(char *value);

inline union ht_value
ht_ulong_value(unsigned long value);


/* Functions to dynamically allocate and free string keys and values. */

inline struct ht_key
ht_alloc_str_key(char const *value);

inline union ht_value
ht_alloc_str_value(char const *value);

inline void
ht_free_str_key(struct ht_key key);

inline void
ht_free_str_value(union ht_value value);


/* Equality functions for keys. */

inline bool
ht_equal_const_str_keys(struct ht_key first, struct ht_key second);

inline bool
ht_equal_long_keys(struct ht_key first, struct ht_key second);

inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second);

inline bool
ht_equal_ulong_keys(struct ht_key first, struct ht_key second);


/* Equality functions for values. */

inline bool
ht_equal_const_str_values(union ht_value first, union ht_value second);

inline bool
ht_equal_long_values(union ht_value first, union ht_value second);

inline bool
ht_equal_str_values(union ht_value first, union ht_value second);

inline bool
ht_equal_ulong_values(union ht_value first, union ht_value second);


/* Hash functions. */

unsigned
ht_hash_of_const_str(char const *value);

inline unsigned
ht_hash_of_long(long value);

inline unsigned
ht_hash_of_str(char *value);

inline unsigned
ht_hash_of_ulong(unsigned long value);


/* Definitions of inline functions. */

inline struct hashtable *
hashtable_alloc(int capacity, ht_equal_keys_func *equal_keys)
{
  struct hashtable *hashtable = malloc(hashtable_size(capacity));
  if (hashtable) hashtable_init(hashtable, capacity, equal_keys);
  return hashtable;
}


inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable)
{
  struct ht_entry *entries = malloc(hashtable->count * sizeof(struct ht_entry));
  if (entries) hashtable_copy_entries(hashtable, entries, hashtable->count);
  return entries;
}


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


inline void
hashtable_free(struct hashtable *hashtable)
{
  free(hashtable);
}


inline size_t
hashtable_size(int capacity)
{
  return sizeof(struct hashtable) + sizeof(struct ht_bucket[capacity]);
}


inline struct ht_key
ht_alloc_str_key(char const *value)
{
  return (struct ht_key){
    .hash=ht_hash_of_const_str(value),
    .value=ht_alloc_str_value(value),
  };
}


inline union ht_value
ht_alloc_str_value(char const *value)
{
  return (union ht_value){.str_value=strdup(value),};
}


inline struct ht_key
ht_const_str_key(char const *value)
{
  return (struct ht_key){
    .hash=ht_hash_of_const_str(value),
    .value=ht_const_str_value(value),
  };
}


inline union ht_value
ht_const_str_value(char const *value)
{
  return (union ht_value){.const_str_value=value,};
}


inline bool
ht_equal_const_str_keys(struct ht_key first, struct ht_key second)
{
  return ht_equal_const_str_values(first.value, second.value);
}


inline bool
ht_equal_const_str_values(union ht_value first, union ht_value second)
{
  return 0 == strcmp(first.const_str_value, second.const_str_value);
}


inline bool
ht_equal_long_keys(struct ht_key first, struct ht_key second)
{
  return ht_equal_long_values(first.value, second.value);
}


inline bool
ht_equal_long_values(union ht_value first, union ht_value second)
{
  return first.long_value == second.long_value;
}


inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second)
{
  return ht_equal_str_values(first.value, second.value);
}


inline bool
ht_equal_str_values(union ht_value first, union ht_value second)
{
  return 0 == strcmp(first.str_value, second.str_value);
}


inline bool
ht_equal_ulong_keys(struct ht_key first, struct ht_key second)
{
  return ht_equal_ulong_values(first.value, second.value);
}


inline bool
ht_equal_ulong_values(union ht_value first, union ht_value second)
{
  return first.ulong_value == second.ulong_value;
}


inline void
ht_free_str_key(struct ht_key key)
{
  ht_free_str_value(key.value);
}


inline void
ht_free_str_value(union ht_value value)
{
  free(value.str_value);
}


inline unsigned
ht_hash_of_long(long value)
{
  return ht_hash_of_ulong((unsigned long)value);
}


inline unsigned
ht_hash_of_str(char *value)
{
  return ht_hash_of_const_str(value);
}


inline unsigned
ht_hash_of_ulong(unsigned long value)
{
  return value % UINT_MAX;
}


inline struct ht_key
ht_long_key(long value)
{
  return (struct ht_key){
    .hash=ht_hash_of_long(value),
    .value=ht_long_value(value),
  };
}


inline union ht_value
ht_long_value(long value)
{
  return (union ht_value){.long_value=value,};
}


inline struct ht_key
ht_str_key(char *value)
{
  return (struct ht_key){
    .hash=ht_hash_of_str(value),
    .value=ht_str_value(value),
  };
}


inline union ht_value
ht_str_value(char *value)
{
  return (union ht_value){.str_value=value,};
}


inline struct ht_key
ht_ulong_key(unsigned long value)
{
  return (struct ht_key){
    .hash=ht_hash_of_ulong(value),
    .value=ht_ulong_value(value),
  };
}


inline union ht_value
ht_ulong_value(unsigned long value)
{
  return (union ht_value){.ulong_value=value,};
}


#endif

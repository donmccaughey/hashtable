#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/* Pointer-sized integers. */

typedef intptr_t ht_int_t;
typedef uintptr_t ht_uint_t;


/* A value contains a pointer-sized type. */
union ht_value {
  void const *const_ptr_value;
  char const *const_str_value;
  ht_int_t int_value;
  void *ptr_value;
  char *str_value;
  ht_uint_t uint_value;
};


/* A key contains a value and the computed hash of that value. */
struct ht_key {
  size_t hash;
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
  size_t capacity;
  size_t count;
  ht_equal_keys_func *equal_keys;
  void *user_data;
  struct ht_bucket buckets[];
};


/* Hash table creation and destruction functions. */

inline struct hashtable *
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys);

inline void
hashtable_free(struct hashtable *hashtable);

void
hashtable_init(struct hashtable *hashtable,
               size_t capacity,
               ht_equal_keys_func *equal_keys);

inline size_t
hashtable_size(size_t capacity);


/* Functions to read, write and delete entries in the hash table. */

struct ht_entry const *
hashtable_get(struct hashtable const *hashtable, struct ht_key key);

int
hashtable_set(struct hashtable *hashtable,
              struct ht_key key,
              union ht_value value,
              bool *had_entry,
              struct ht_entry *entry_out);

int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 struct ht_entry *entry_out);


/* Functions to iterate over keys and values in the hash table. */

struct ht_entry const *
hashtable_next(struct hashtable const *hashtable, size_t *iterator);

inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable);

inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable);

inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable);

size_t
hashtable_copy_keys(struct hashtable const *hashtable,
                    struct ht_key *keys,
                    size_t keys_count);

size_t
hashtable_copy_values(struct hashtable const *hashtable,
                      union ht_value *values,
                      size_t values_count);


size_t
hashtable_copy_entries(struct hashtable const *hashtable,
                       struct ht_entry *entries,
                       size_t entries_count);


/* Functions to make keys. */

inline struct ht_key
ht_const_str_key(char const *value);

inline struct ht_key
ht_int_key(ht_int_t value);

inline struct ht_key
ht_str_key(char *value);

inline struct ht_key
ht_uint_key(ht_uint_t value);


/* Functions to make values. */

inline union ht_value
ht_const_str_value(char const *value);

inline union ht_value
ht_int_value(ht_int_t value);

inline union ht_value
ht_str_value(char *value);

inline union ht_value
ht_uint_value(ht_uint_t value);


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
ht_equal_int_keys(struct ht_key first, struct ht_key second);

inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second);

inline bool
ht_equal_uint_keys(struct ht_key first, struct ht_key second);


/* Equality functions for values. */

inline bool
ht_equal_const_str_values(union ht_value first, union ht_value second);

inline bool
ht_equal_int_values(union ht_value first, union ht_value second);

inline bool
ht_equal_str_values(union ht_value first, union ht_value second);

inline bool
ht_equal_uint_values(union ht_value first, union ht_value second);


/* Hash functions. */

size_t
ht_hash_of_const_str(char const *value);

inline size_t
ht_hash_of_int(ht_int_t value);

inline size_t
ht_hash_of_str(char *value);

inline size_t
ht_hash_of_uint(ht_uint_t value);


/* Definitions of inline functions. */

inline struct hashtable *
hashtable_alloc(size_t capacity, ht_equal_keys_func *equal_keys)
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
hashtable_size(size_t capacity)
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
ht_equal_int_keys(struct ht_key first, struct ht_key second)
{
  return ht_equal_int_values(first.value, second.value);
}


inline bool
ht_equal_int_values(union ht_value first, union ht_value second)
{
  return first.int_value == second.int_value;
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
ht_equal_uint_keys(struct ht_key first, struct ht_key second)
{
  return ht_equal_uint_values(first.value, second.value);
}


inline bool
ht_equal_uint_values(union ht_value first, union ht_value second)
{
  return first.uint_value == second.uint_value;
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


inline size_t
ht_hash_of_int(ht_int_t value)
{
  return (ht_uint_t)value % SIZE_MAX;
}


inline size_t
ht_hash_of_str(char *value)
{
  return ht_hash_of_const_str(value);
}


inline size_t
ht_hash_of_uint(ht_uint_t value)
{
  return value % SIZE_MAX;
}


inline struct ht_key
ht_int_key(ht_int_t value)
{
  return (struct ht_key){
    .hash=ht_hash_of_int(value),
    .value=ht_int_value(value),
  };
}


inline union ht_value
ht_int_value(ht_int_t value)
{
  return (union ht_value){.int_value=value,};
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
ht_uint_key(ht_uint_t value)
{
  return (struct ht_key){
    .hash=ht_hash_of_uint(value),
    .value=ht_uint_value(value),
  };
}


inline union ht_value
ht_uint_value(ht_uint_t value)
{
  return (union ht_value){.uint_value=value,};
}


#endif

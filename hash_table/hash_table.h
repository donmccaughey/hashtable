#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct hash_table;


union ht_value {
  void const *const_ptr_value;
  char const *const_str_value;
  intptr_t int_value;
  void *ptr_value;
  char *str_value;
  uintptr_t uint_value;
};


struct ht_key {
  size_t hash;
  union ht_value value;
};


typedef bool
ht_equals_func(struct ht_key first_key, struct ht_key second_key);


struct hash_table *
hash_table_alloc(size_t capacity, ht_equals_func *equals);

struct ht_key *
hash_table_alloc_keys(struct hash_table const *hash_table);

union ht_value *
hash_table_alloc_values(struct hash_table const *hash_table);

size_t
hash_table_capacity(struct hash_table const *hash_table);

size_t
hash_table_count(struct hash_table const *hash_table);

void
hash_table_free(struct hash_table *hash_table);

int
hash_table_get(struct hash_table const *hash_table,
               struct ht_key key,
               union ht_value *value_out);

int
hash_table_put(struct hash_table *hash_table,
               struct ht_key key,
               union ht_value value,
               union ht_value *previous_value_out);

int
hash_table_remove(struct hash_table *hash_table,
                  struct ht_key key,
                  union ht_value *previous_value_out);

inline struct ht_key
ht_alloc_str_key(char const *value);

inline union ht_value
ht_alloc_str_value(char const *value);

inline struct ht_key
ht_const_str_key(char const *value);

inline union ht_value
ht_const_str_value(char const *value);

bool
ht_equal_const_str_keys(struct ht_key first_key, struct ht_key second_key);

bool
ht_equal_int_keys(struct ht_key first_key, struct ht_key second_key);

bool
ht_equal_str_keys(struct ht_key first_key, struct ht_key second_key);

bool
ht_equal_uint_keys(struct ht_key first_key, struct ht_key second_key);

inline void
ht_free_str_key(struct ht_key key);

inline void
ht_free_str_value(union ht_value value);

size_t
ht_hash_const_str(char const *value);

inline size_t
ht_hash_int(intptr_t value);

inline size_t
ht_hash_str(char *value);

inline size_t
ht_hash_uint(uintptr_t value);

inline struct ht_key
ht_int_key(intptr_t value);

inline union ht_value
ht_int_value(intptr_t value);

inline struct ht_key
ht_str_key(char *value);

inline union ht_value
ht_str_value(char *value);

inline struct ht_key
ht_uint_key(uintptr_t value);

inline union ht_value
ht_uint_value(uintptr_t value);


inline struct ht_key
ht_alloc_str_key(char const *value)
{
  return (struct ht_key){
    .hash=ht_hash_const_str(value),
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
    .hash=ht_hash_const_str(value),
    .value=ht_const_str_value(value),
  };
}


inline union ht_value
ht_const_str_value(char const *value)
{
  return (union ht_value){.const_str_value=value,};
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
ht_hash_int(intptr_t value)
{
  return (uintptr_t)value % SIZE_MAX;
}


inline size_t
ht_hash_str(char *value)
{
  return ht_hash_const_str(value);
}


inline size_t
ht_hash_uint(uintptr_t value)
{
  return value % SIZE_MAX;
}


inline struct ht_key
ht_int_key(intptr_t value)
{
  return (struct ht_key){
    .hash=ht_hash_int(value),
    .value=ht_int_value(value),
  };
}


inline union ht_value
ht_int_value(intptr_t value)
{
  return (union ht_value){.int_value=value,};
}


inline struct ht_key
ht_str_key(char *value)
{
  return (struct ht_key){
    .hash=ht_hash_str(value),
    .value=ht_str_value(value),
  };
}


inline union ht_value
ht_str_value(char *value)
{
  return (union ht_value){.str_value=value,};
}


inline struct ht_key
ht_uint_key(uintptr_t value)
{
  return (struct ht_key){
    .hash=ht_hash_uint(value),
    .value=ht_uint_value(value),
  };
}


inline union ht_value
ht_uint_value(uintptr_t value)
{
  return (union ht_value){.uint_value=value,};
}


#endif

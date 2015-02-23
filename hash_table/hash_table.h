#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>


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


#endif

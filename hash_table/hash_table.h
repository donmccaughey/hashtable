#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>


#define HT_UINT_MAX UINT32_MAX


struct hash_table;


typedef uint32_t ht_uint_t;

typedef bool
ht_equals_function(void const *first_key, void const *second_key);

typedef ht_uint_t
ht_hash_function(void const *key);


struct hash_table *
hash_table_alloc(ht_uint_t capacity,
                 ht_hash_function *hash,
                 ht_equals_function *equals);

void const **
hash_table_alloc_keys(struct hash_table const *hash_table);

ht_uint_t
hash_table_capacity(struct hash_table const *hash_table);

ht_uint_t
hash_table_count(struct hash_table const *hash_table);

void
hash_table_free(struct hash_table *hash_table);

void const *
hash_table_get(struct hash_table const *hash_table, void const *key);

bool
hash_table_has_key(struct hash_table const *hash_table, void const *key);

int
hash_table_put(struct hash_table *hash_table,
               void const *key,
               void const *value,
               void const **previous_value);

void
hash_table_remove(struct hash_table *hash_table,
                  void const *key,
                  void const **previous_value);


#endif

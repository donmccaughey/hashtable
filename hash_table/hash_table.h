#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>


struct hash_table;


typedef uint32_t hash_table_uint_t;

typedef bool
hash_table_equals_function(void const *first_key, void const *second_key);

typedef hash_table_uint_t
hash_table_hash_function(void const *key);


struct hash_table *
hash_table_alloc(hash_table_uint_t capacity,
                 hash_table_hash_function *hash,
                 hash_table_equals_function *equals);

void const **
hash_table_alloc_keys(struct hash_table const *hash_table);

hash_table_uint_t
hash_table_capacity(struct hash_table const *hash_table);

hash_table_uint_t
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

void const *
hash_table_remove(struct hash_table *hash_table, void const *key);


#endif

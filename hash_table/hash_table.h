#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>


struct hash_table;


typedef int
hash_table_compare_keys_function(void *first_key, void *second_key);

typedef uint32_t
hash_table_hash_key_function(void *key);


struct hash_table *
hash_table_alloc(uint32_t capacity,
                 hash_table_hash_key_function *hash_key,
                 hash_table_compare_keys_function *compare_keys);

uint32_t
hash_table_capacity(struct hash_table const *hash_table);

uint32_t
hash_table_count(struct hash_table const *hash_table);

void
hash_table_free(struct hash_table *hash_table);

void *
hash_table_get(struct hash_table const *hash_table, void *key);

bool
hash_table_has_key(struct hash_table const *hash_table, void *key);

int
hash_table_put(struct hash_table *hash_table,
               void *key,
               void *value,
               void **previous_value);

void *
hash_table_remove(struct hash_table *hash_table, void *key);


#endif

#include "stringtable.h"

#include "hashtable.h"


static inline union ht_value
alloc_str_value(char const *string)
{
    return (union ht_value){ .str_value=strdup(string) };
}


static inline struct ht_key
alloc_str_key(char const *string)
{
    return (struct ht_key){
        .hash=ht_hash_of_const_str(string),
        .value=alloc_str_value(string)
    };
}


static inline void
free_str_value(union ht_value value)
{
    free(value.str_value);
}


static inline void
free_str_key(struct ht_key key)
{
    free_str_value(key.value);
}


static inline void
free_str_entry(struct hashtable *hashtable, struct ht_entry entry)
{
    ((void)hashtable);
    free_str_key(entry.key);
    free_str_value(entry.value);
}


struct stringtable *
stringtable_alloc(int initial_capacity)
{
    struct stringtable *stringtable = malloc(sizeof(struct stringtable));
    if (!stringtable) return NULL;
    
    stringtable->hashtable = hashtable_alloc(initial_capacity,
                                             ht_equal_str_keys);
    if (!stringtable->hashtable) {
        free(stringtable);
        return NULL;
    }
    
    return stringtable;
}


st_entry_t *
stringtable_alloc_entries(struct stringtable const *stringtable)
{
    st_entry_t *entries = calloc(stringtable->hashtable->count,
                                 sizeof(st_entry_t));
    if (!entries) return NULL;
    
    struct ht_entry const *entry;
    int iterator = 0;
    for (int i = 0; i < stringtable->hashtable->count; ++i) {
        entry = hashtable_next(stringtable->hashtable, &iterator);
        entries[i][0] = strdup(entry->key.value.str_value);
        entries[i][1] = strdup(entry->value.str_value);
        if (!entries[i][0] || !entries[i][1]) {
            while (i > 0) {
                free(entries[i][0]);
                free(entries[i][1]);
                --i;
            }
            free(entries);
            return NULL;
        }
    }
    
    return entries;
}


char **
stringtable_alloc_keys(struct stringtable const *stringtable)
{
    char **keys = calloc(stringtable->hashtable->count, sizeof(char *));
    if (!keys) return NULL;
    
    struct ht_entry const *entry;
    int iterator = 0;
    for (int i = 0; i < stringtable->hashtable->count; ++i) {
        entry = hashtable_next(stringtable->hashtable, &iterator);
        keys[i] = strdup(entry->key.value.str_value);
        if (!keys[i]) {
            while (i > 0) free(keys[--i]);
            free(keys);
            return NULL;
        }
    }
    
    return keys;
}


char **
stringtable_alloc_values(struct stringtable const *stringtable)
{
    char **values = calloc(stringtable->hashtable->count, sizeof(char *));
    if (!values) return NULL;
    
    struct ht_entry const *entry;
    int iterator = 0;
    for (int i = 0; i < stringtable->hashtable->count; ++i) {
        entry = hashtable_next(stringtable->hashtable, &iterator);
        values[i] = strdup(entry->value.str_value);
        if (!values[i]) {
            while (i > 0) free(values[--i]);
            free(values);
            return NULL;
        }
    }
    
    return values;
}


int
stringtable_capacity(struct stringtable const *stringtable)
{
    return stringtable->hashtable->capacity;
}


void
stringtable_clear(struct stringtable *stringtable)
{
    hashtable_clear(stringtable->hashtable, free_str_entry);
}


int
stringtable_count(struct stringtable const *stringtable)
{
    return stringtable->hashtable->count;
}


void
stringtable_free(struct stringtable *stringtable)
{
    hashtable_free(stringtable->hashtable, free_str_entry);
    free(stringtable);
}


char const *
stringtable_get(struct stringtable const *stringtable, char const *key)
{
    struct ht_entry const *entry = hashtable_get(stringtable->hashtable,
                                                 ht_const_str_key(key));
    return entry ? entry->value.str_value : NULL;
}


bool
stringtable_next(struct stringtable const *stringtable,
                 int *iterator,
                 char const **key_out,
                 char const **value_out)
{
    struct ht_entry const *entry = hashtable_next(stringtable->hashtable,
                                                  iterator);
    if (entry) {
        if (key_out) *key_out = entry->key.value.str_value;
        if (value_out) *value_out = entry->value.str_value;
        return true;
    }
    return false;
}


int
stringtable_remove(struct stringtable *stringtable, char const *key)
{
    struct ht_key const_str_key = ht_const_str_key(key);
    struct ht_entry entry;
    if (0 == hashtable_remove(stringtable->hashtable, const_str_key, &entry)) {
        free_str_entry(stringtable->hashtable, entry);
        return 0;
    }
    return -1;
}


int
stringtable_set(struct stringtable *stringtable,
                char const *key,
                char const *value)
{
    // TODO: extract this test as hashtable_overloaded()
    // TODO: correctly handle overflow
    int max_count = stringtable->hashtable->capacity / 16 * 11;
    if (stringtable->hashtable->count > max_count) {
        // TODO: extract this block as hashtable_realloc()
        // TODO: correctly handle overflow
        int capacity = stringtable->hashtable->capacity / 2 * 3;
        struct hashtable *hashtable
                = hashtable_alloc(capacity, stringtable->hashtable->equal_keys);
        if (!hashtable) return -1;
        
        hashtable_update(hashtable, stringtable->hashtable, NULL, NULL);
        
        hashtable_free(stringtable->hashtable, NULL);
        stringtable->hashtable = hashtable;
    }
    
    struct ht_key str_key = alloc_str_key(key);
    if (!str_key.value.str_value) return -1;
    
    union ht_value str_value = alloc_str_value(value);
    if (!str_value.str_value) {
        free_str_key(str_key);
        return -1;
    }
    
    bool had_entry;
    struct ht_entry entry;
    int result = hashtable_set(stringtable->hashtable, str_key, str_value,
                               &had_entry, &entry);
    if (-1 == result) {
        free_str_key(str_key);
        free_str_value(str_value);
        return -1;
    }
    
    if (had_entry) free_str_entry(stringtable->hashtable, entry);
    return 0;
}


int
stringtable_update(struct stringtable *destination,
                   struct stringtable const *source)
{
    // TODO: reimplement using hashtable_realloc() and hashtable_update()
    int iterator = 0;
    char const *key;
    char const *value;
    while (stringtable_next(source, &iterator, &key, &value)) {
        if (-1 == stringtable_set(destination, key, value)) return -1;
    }
    return 0;
}

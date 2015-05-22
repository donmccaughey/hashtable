#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED


#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// A value contains a pointer type or a long integer type.
union ht_value {
  void const *const_ptr_value;
  char const *const_str_value;
  long long_value;
  void *ptr_value;
  char *str_value;
  unsigned long ulong_value;
};


// A key contains a value and the computed hash of that value.
struct ht_key {
  unsigned hash;
  union ht_value value;
};


// An entry contains an key and corresponding value.
struct ht_entry {
  struct ht_key key;
  union ht_value value;
};


// A space for an entry in the hash table, which may be used or unused.
struct ht_bucket {
  bool in_use;
  struct ht_entry entry;
};


// Equality function type for keys.
typedef bool
ht_equal_keys_func(struct ht_key first, struct ht_key second);


// The hash table.
struct hashtable {
  int capacity;
  int count;
  ht_equal_keys_func *equal_keys;
  void *user_data;
  struct ht_bucket buckets[];
};


// Initialize a hash table.
//
// `capacity' is the maximum number of entries that the hashtable will hold.
// Performance of the hash table decreases significantly when the number of
// entries stored exceeds about 70% of `capacity'.
void
hashtable_init(struct hashtable *hashtable,
               int capacity,
               ht_equal_keys_func *equal_keys);


// Compute the size in bytes required for a hash table of a given capacity.
inline size_t
hashtable_size(int capacity)
{
  return sizeof(struct hashtable) + sizeof(struct ht_bucket[capacity]);
}


// Allocate and initialize a hash table.
//
// Returns NULL if memory cannot be allocated.
inline struct hashtable *
hashtable_alloc(int capacity, ht_equal_keys_func *equal_keys)
{
  struct hashtable *hashtable = malloc(hashtable_size(capacity));
  if (hashtable) hashtable_init(hashtable, capacity, equal_keys);
  return hashtable;
}


// Free a previously allocated hash table.
//
// `hashtable' may be NULL.
inline void
hashtable_free(struct hashtable *hashtable)
{
  free(hashtable);
}


// Read an entry from a hash table.
//
// Returns NULL if the hash table does not contain an entry for the key.
struct ht_entry const *
hashtable_get(struct hashtable const *hashtable, struct ht_key key);


// Write an entry to a hash table.
//
// `replaced' and `entry' may be NULL. If the hash table did not contain an
// entry for the key, on return `replaced' contains false; if the hash table
// contained an entry for the key, on return `replaced' contains true and
// `entry' contains the original key and value.
//
// Returns 0 if an entry was written to the hash table, or -1 if the hash table
// is full.
int
hashtable_set(struct hashtable *hashtable,
              struct ht_key key,
              union ht_value value,
              bool *replaced,
              struct ht_entry *entry);


// Delete an entry in a hash table.
//
// `entry' may be NULL. If the hash table contained an entry for the key, on
// return `entry' contains the original key and value.
//
// Returns 0 if the entry was deleted, or -1 if the hash table does not contain
// an entry for the key.
int
hashtable_remove(struct hashtable *hashtable,
                 struct ht_key key,
                 struct ht_entry *entry);


// Iterate over entries in a hash table.
//
// Do not modify the hash table while iterating.
//
// `iterator' must point to an int value. Set the int value to 0 before the
// first call to the function.
//
// Returns a pointer to the next entry in the hash table, or NULL when there
// are no more entries.
struct ht_entry const *
hashtable_next(struct hashtable const *hashtable, int *iterator);


// Copy the keys from a hash table into an array.
//
// Returns the count if items copied into the `keys' array, which will not be
// more than `keys_count'.
int
hashtable_copy_keys(struct hashtable const *hashtable,
                    struct ht_key *keys,
                    int keys_count);


// Copy the values from a hash table into an array.
//
// Returns the count if items copied into the `values' array, which will not be
// more than `values_count'.
int
hashtable_copy_values(struct hashtable const *hashtable,
                      union ht_value *values,
                      int values_count);


// Copy the entries from a hash table into an array.
//
// Returns the count if items copied into the `entries' array, which will not
// be more than `entries_count'.
int
hashtable_copy_entries(struct hashtable const *hashtable,
                       struct ht_entry *entries,
                       int entries_count);


// Allocate an array containing the keys from a hash table.
//
// The array will contain `hashtable->count' items. The caller is responsible
// for freeing the array.
//
// Returns NULL if memory cannot be allocated.
inline struct ht_key *
hashtable_alloc_keys(struct hashtable const *hashtable)
{
  struct ht_key *keys = malloc(hashtable->count * sizeof(struct ht_key));
  if (keys) hashtable_copy_keys(hashtable, keys, hashtable->count);
  return keys;
}


// Allocate an array containing the values from a hash table.
//
// The array will contain `hashtable->count' items. The caller is responsible
// for freeing the array.
//
// Returns NULL if memory cannot be allocated.
inline union ht_value *
hashtable_alloc_values(struct hashtable const *hashtable)
{
  union ht_value *values = malloc(hashtable->count * sizeof(union ht_value));
  if (values) hashtable_copy_values(hashtable, values, hashtable->count);
  return values;
}


// Allocate an array containing the entries from a hash table.
//
// The array will contain `hashtable->count' items. The caller is responsible
// for freeing the array.
//
// Returns NULL if memory cannot be allocated.
inline struct ht_entry *
hashtable_alloc_entries(struct hashtable const *hashtable)
{
  struct ht_entry *entries = malloc(hashtable->count * sizeof(struct ht_entry));
  if (entries) hashtable_copy_entries(hashtable, entries, hashtable->count);
  return entries;
}


// Make a value from a const string.
inline union ht_value
ht_const_str_value(char const *value)
{
  return (union ht_value){ .const_str_value = value };
}


// Make a value from a long int.
inline union ht_value
ht_long_value(long value)
{
  return (union ht_value){ .long_value = value };
}


// Make a value from a string.
inline union ht_value
ht_str_value(char *value)
{
  return (union ht_value){ .str_value = value };
}


// Make a value from an unsigned long int.
inline union ht_value
ht_ulong_value(unsigned long value)
{
  return (union ht_value){ .ulong_value = value };
}


// Calculate the hash for an unsigned long int.
inline unsigned
ht_hash_of_ulong(unsigned long value)
{
  return value % UINT_MAX;
}


// Calculate the hash for a long int.
inline unsigned
ht_hash_of_long(long value)
{
  return ht_hash_of_ulong((unsigned long)value);
}


// Calculate the hash for a const string.
unsigned
ht_hash_of_const_str(char const *value);


// Calculate the hash for a string.
inline unsigned
ht_hash_of_str(char *value)
{
  return ht_hash_of_const_str(value);
}


// Make a key from a const string.
inline struct ht_key
ht_const_str_key(char const *value)
{
  return (struct ht_key){
    .hash = ht_hash_of_const_str(value),
    .value = ht_const_str_value(value)
  };
}


// Make a key from a long int.
inline struct ht_key
ht_long_key(long value)
{
  return (struct ht_key){
    .hash = ht_hash_of_long(value),
    .value = ht_long_value(value)
  };
}


// Make a key from a string.
inline struct ht_key
ht_str_key(char *value)
{
  return (struct ht_key){
    .hash = ht_hash_of_str(value),
    .value = ht_str_value(value)
  };
}


// Make a key from an unsigned long.
inline struct ht_key
ht_ulong_key(unsigned long value)
{
  return (struct ht_key){
    .hash = ht_hash_of_ulong(value),
    .value = ht_ulong_value(value)
  };
}


// Compare two const string keys for equality.
inline bool
ht_equal_const_str_keys(struct ht_key first, struct ht_key second)
{
  return first.hash == second.hash
      && 0 == strcmp(first.value.const_str_value, second.value.const_str_value);
}


// Compare two long int keys for equality.
inline bool
ht_equal_long_keys(struct ht_key first, struct ht_key second)
{
  return first.value.long_value == second.value.long_value;
}


// Compare two string keys for equality.
inline bool
ht_equal_str_keys(struct ht_key first, struct ht_key second)
{
  return first.hash == second.hash
      && 0 == strcmp(first.value.str_value, second.value.str_value);
}


// Compare two unsigned long int keys for equality.
inline bool
ht_equal_ulong_keys(struct ht_key first, struct ht_key second)
{
  return first.value.ulong_value == second.value.ulong_value;
}


#endif

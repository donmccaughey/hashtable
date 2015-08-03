#ifndef STRINGTABLE_H_INCLUDED
#define STRINGTABLE_H_INCLUDED


#include <stdbool.h>


struct hashtable;


struct stringtable {
    struct hashtable *hashtable;
};


// An entry copied from a string table.
//
// The key is at index 0 and the value at index 1.
typedef char *st_entry_t[2];


/****************************
 * Creation and destruction *
 ****************************/

// Allocate a string table.
//
// Returns NULL if memory cannot be allocated.
struct stringtable *
stringtable_alloc(int initial_capacity);

// Delete all entries and free a string table.
void
stringtable_free(struct stringtable *stringtable);


/***************
 * Information *
 ***************/

// Get the current capacity of a string table.
int
stringtable_capacity(struct stringtable const *stringtable);

// Get the number of entries in a string table.
int
stringtable_count(struct stringtable const *stringtable);


/**********************************
 * Read, write and delete entries *
 **********************************/

// Read an entry from a string table.
//
// Returns NULL if the string table does not contain an entry for the key.
char const *
stringtable_get(struct stringtable const *stringtable, char const *key);

// Write an entry to a string table.
//
// If the string table contains an entry for the key, it is replaced.  The
// string table will increase in capacity if needed to accomodate a new entry.
// The string table makes copies of the key and value.
//
// Returns 0 if an entry was written to the string table, or -1 if memory
// cannot be allocated for the new entry.
int
stringtable_set(struct stringtable *stringtable,
                char const *key,
                char const *value);

// Delete an entry in a string table.
//
// Returns 0 if the entry was deleted, or -1 if the string table does not
// contain an entry for the key.
int
stringtable_remove(struct stringtable *stringtable, char const *key);

// Delete all entries in a string table.
void
stringtable_clear(struct stringtable *stringtable);

// Copy entries from one string table into another.
//
// Entries with equal keys from `source' replace entries in `destination'.
// The `destination' string table will increase in capacity if needed to
// accomodate new entries.
//
// Returns 0 if all entries in `source' are copied into `destination', or -1 if
// memory cannot be allocated for new or replaced entries.
int
stringtable_update(struct stringtable *destination,
                   struct stringtable const *source);


/*************
 * Iteration *
 *************/

// Iterate over entries in a string table.
//
// Do not modify the string table while iterating. `*iterator' must contain 0
// before the first call to the function. Do not modify the value of
// `*iterator' between calls, except to set it to 0 to restart iteration from
// the beginning.
//
// Returns `true' if the next entry was found, in which case `key_out' and
// `value_out' will point to the next key and value strings, respectively.
// Returns `false' when there are no more entries.
bool
stringtable_next(struct stringtable const *stringtable,
                 int *iterator,
                 char const **key_out,
                 char const **value_out);


/***********************************
 * Get all keys, values or entries *
 ***********************************/

// Allocate an array containing a copy of the entries from a string table.
//
// The array will contain `stringtable_count()' items. The caller is
// responsible for freeing the array and the strings it contains.
//
// Returns NULL if memory cannot be allocated.
st_entry_t *
stringtable_alloc_entries(struct stringtable const *stringtable);

// Allocate an array containing a copy of the keys from a string table.
//
// The array will contain `stringtable_count()' items. The caller is
// responsible for freeing the array and the strings it contains.
//
// Returns NULL if memory cannot be allocated.
char **
stringtable_alloc_keys(struct stringtable const *stringtable);

// Allocate an array containing a copy of the values from a string table.
//
// The array will contain `stringtable_count()' items. The caller is
// responsible for freeing the array and the strings it contains.
//
// Returns NULL if memory cannot be allocated.
char **
stringtable_alloc_values(struct stringtable const *stringtable);


#endif

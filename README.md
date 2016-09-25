hashtable
=========

A hash table in C99.

This hash table uses [open addressing with linear probing][1] to resolve hash 
collisions. It may be used without dynamic memory allocation, though 
convenience `alloc` functions are included. The capacity of the hash table is 
determined when it is initialized; it does not automatically grow or shrink as 
entries are inserted or removed. Because it uses open addressing, performance 
may decrease significantly when the number of entries in the hash table exceeds 
about 70% of capacity.

Keys may be `long` or `unsigned long` integers, string pointers or custom types 
(as `void` pointers). The user is responsible for managing the lifetime of 
string and `void` pointer keys. Hash functions and equality functions for 
`long`, `unsigned long` and string pointer keys are included; the user must 
provide hash and equality functions for custom types. Within a hash table, keys 
must all be of the same type.

Value types are the same as for keys (`long`, `unsigned long`, strings and 
`void` pointers). The user is responsible for managing the lifetime of string 
and `void` pointer values. Within a hash table, values must all be of the same 
type (which may be different than the key type).

Simple Example
--------------
Here is a short code snippet that allocates a `hashtable` with a capacity of
ten entries, then adds three entries and does a lookup.  Note that in this 
example, keys and values are `const` strings; the `hashtable` expects them to
remain valid and unchanged while they are in use.
    
    #include <hashtable.h>
    #include <assert.h>
    
    struct hashtable *hashtable = hashtable_alloc(10, ht_equal_const_str_keys);
    assert(hashtable);

    int result = hashtable_set(hashtable, 
                               ht_const_str_key("rat"),
                               ht_const_str_value("Rattus norvegicus"),
                               NULL, 
                               NULL);
    assert(result == 0);

    result = hashtable_set(hashtable, 
                           ht_const_str_key("cat"),
                           ht_const_str_value("Felis catus"), 
                           NULL, 
                           NULL);
    assert(result == 0);

    result = hashtable_set(hashtable, 
                           ht_const_str_key("dog"),
                           ht_const_str_value("Canis familiaris"), 
                           NULL, 
                           NULL);
    assert(result == 0);

    struct ht_entry const *entry = hashtable_get(hashtable,
                                                 ht_const_str_key("cat"));
    printf("The %s is species %s.\n",
           entry->key.value.const_str_value, 
           entry->value.const_str_value);
    // prints "The cat is species Felis catus."
    
    hashtable_free(hashtable, NULL);

Hash Algorithm
--------------
The caller is responsible for calculating the hash for keys in a `hashtable`.
An implementation of the public domain [FNV-1a hash algorithm][2] is provided.
The provided hashing functions (`ht_hash_of()`, `ht_hash_of_long()`,
`ht_hash_of_str()`, etc.) and key functions (`ht_long_key()`, `ht_str_key()`, 
etc.) use the FNV-1a hash algorithm.

Influences
----------
In [A Probing Hash Table Framework][3], Chase Geigle describes the development of a hash table framework in C++.
In [Types Don't Know #][4], the authors describe a technique in C++ to decouple hashing algorithms from types.

License
-------
`hashtable` is made available under a BSD-style license; see the LICENSE file 
for details.


[1]: https://en.wikipedia.org/wiki/Hash_table#Open_addressing "Open Addressing"
[2]: http://www.isthe.com/chongo/tech/comp/fnv/index.html "FNV Hash"
[3]: https://chara.cs.illinois.edu/sites/cgeigle/blog/2016/01/29/a-probing-hash-table-framework/ "A Probing Hash Table Framework"
[4]: https://isocpp.org/files/papers/n3980.html#generalpurpose "Types Don't Know #"

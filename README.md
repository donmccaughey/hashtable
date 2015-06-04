hashtable
=========

A hash table in C99.

This hash table uses [open addressing with linear probing][1] to resolve hash 
collisions. It may be used without dynamic memory allocation, though 
convenience `alloc` functions are included. The capacity of the hash table is 
determined when it is initialized; it does not automatically grow or shrink as 
entries are inserted or removed. Because it uses open addressing, performance 
decreases significantly when the number of entries in the hash table exceeds 
about 70% of capacity.

Keys may be `long` or `unsigned long` integers, string pointers or custom types 
(as `void` pointers). The user is responsible for managing the lifetime of 
items referenced by string and `void` pointer keys. Hash functions and equality 
functions for `long`, `unsigned long` and string pointer keys are included; the 
user must provide hash and equality functions for custom types. Within a hash 
table, keys must all be of the same type.

Value types are the same as for keys (`long`, `unsigned long`, strings and 
`void` pointers). The user is responsible for managing the lifetime of items
referenced by string and `void` pointer values. Within a hash table, values 
must all be of the same type (which may be different than the key type).

License
-------
`hashtable` is made available under a BSD-style license; see the LICENSE file 
for details.


[1]: https://en.wikipedia.org/wiki/Hash_table#Open_addressing "Open Addressing"

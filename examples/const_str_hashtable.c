#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"


int
main(void)
{
    struct hashtable *hashtable = hashtable_alloc(10, ht_equal_const_str_keys);
    
    int result = hashtable_set(hashtable, ht_const_str_key("rat"),
                               ht_const_str_value("Rattus norvegicus"),
                               NULL, NULL);
    assert(result == 0);
    
    result = hashtable_set(hashtable, ht_const_str_key("cat"),
                           ht_const_str_value("Felis catus"), NULL, NULL);
    assert(result == 0);
    
    result = hashtable_set(hashtable, ht_const_str_key("dog"),
                           ht_const_str_value("Canis familiaris"), NULL, NULL);
    assert(result == 0);
    
    struct ht_entry const *entry = hashtable_get(hashtable,
                                                 ht_const_str_key("cat"));
    printf("The %s is species %s.\n",
           entry->key.value.const_str_value, entry->value.const_str_value);
    
    hashtable_free(hashtable, NULL);
    
    return EXIT_SUCCESS;
}

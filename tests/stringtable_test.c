#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringtable.h"


static int
compare(void const *item1, void const *item2)
{
    char const *const *string1 = item1;
    char const *const *string2 = item2;
    return strcmp(*string1, *string2);
}


static void
test_table_of_two_items(void)
{
    struct stringtable *stringtable = stringtable_alloc(10);
    assert(0 == stringtable_count(stringtable));
    
    char const *value = stringtable_get(stringtable, "red");
    assert(!value);
    
    int result = stringtable_set(stringtable, "red", "apple");
    assert(0 == result);
    assert(1 == stringtable_count(stringtable));
    value = stringtable_get(stringtable, "red");
    assert(0 == strcmp("apple", value));
    
    int iterator = 0;
    char const *key;
    assert(stringtable_next(stringtable, &iterator, &key, &value));
    assert(0 == strcmp("red", key));
    assert(0 == strcmp("apple", value));
    
    assert(!stringtable_next(stringtable, &iterator, &key, &value));
    
    result = stringtable_remove(stringtable, "orange");
    assert(-1 == result);
    assert(1 == stringtable_count(stringtable));
    
    result = stringtable_remove(stringtable, "red");
    assert(0 == result);
    assert(0 == stringtable_count(stringtable));
    
    stringtable_free(stringtable);
}


static void
test_alloc_entries_keys_and_values(void)
{
    char const *colors[] = {
        "red", "orange", "yellow", "blue", "green", "indigo", "violet"
    };
    int const colors_count = sizeof colors / sizeof colors[0];
    qsort(colors, colors_count, sizeof(char const *), compare);
    
    char const *flavors[] = {
        "sweet", "sour", "salty", "bitter", "umami", "chocolate", "coffee"
    };
    int const flavor_count = sizeof flavors / sizeof flavors[0];
    qsort(flavors, flavor_count, sizeof(char const *), compare);
    
    assert(colors_count == flavor_count);
    
    struct stringtable *stringtable = stringtable_alloc(10);
    
    for (int i = 0; i < colors_count; ++i) {
        stringtable_set(stringtable, colors[i], flavors[i]);
    }
    
    assert(colors_count == stringtable_count(stringtable));
    
    char **keys = stringtable_alloc_keys(stringtable);
    for (int i = 0; i < stringtable_count(stringtable); ++i) {
        void *found = bsearch(&keys[i], colors,
                              colors_count, sizeof(char const *), compare);
        assert(found);
    }
    for (int i = 0; i < stringtable_count(stringtable); ++i) free(keys[i]);
    free(keys);
    
    char **values = stringtable_alloc_values(stringtable);
    for (int i = 0; i < stringtable_count(stringtable); ++i) {
        void *found = bsearch(&values[i], flavors,
                              flavor_count, sizeof(char const *), compare);
        assert(found);
    }
    for (int i = 0; i < stringtable_count(stringtable); ++i) free(values[i]);
    free(values);
    
    st_entry_t *entries = stringtable_alloc_entries(stringtable);
    for (int i = 0; i < stringtable_count(stringtable); ++i) {
        void *found = bsearch(&entries[i][0], colors,
                              colors_count, sizeof(char const *), compare);
        assert(found);
        
        found = bsearch(&entries[i][1], flavors,
                        flavor_count, sizeof(char const *), compare);
        assert(found);
    }
    for (int i = 0; i < stringtable_count(stringtable); ++i) {
        free(entries[i][0]);
        free(entries[i][1]);
    }
    free(entries);
    
    stringtable_free(stringtable);
}


static void
test_table_capacity_grows_automatically(void)
{
    struct stringtable *stringtable = stringtable_alloc(10);
    
    for (int i = 0; i < 20; ++i) {
        char *number;
        int result = asprintf(&number, "%i", i);
        assert(result > 0);
        result = stringtable_set(stringtable, number, number);
        assert(result != -1);
        free(number);
    }
    
    assert(20 == stringtable_count(stringtable));
    assert(stringtable_capacity(stringtable) > 10);
    
    for (int i = 20; i < 100; ++i) {
        char *number;
        int result = asprintf(&number, "%i", i);
        assert(result > 0);
        result = stringtable_set(stringtable, number, number);
        assert(result != -1);
        free(number);
    }
    
    assert(100 == stringtable_count(stringtable));
    assert(stringtable_capacity(stringtable) > 100);
    
    stringtable_free(stringtable);
}


void
stringtable_test(void)
{
    test_table_of_two_items();
    test_alloc_entries_keys_and_values();
    test_table_capacity_grows_automatically();
}

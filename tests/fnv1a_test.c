#include <assert.h>

#include "asserts.h"
#include "hashtable.h"

#include <stdio.h>

void
fnv1a_test(void)
{
  struct ht_key key1 = ht_const_str_key(NULL);
  assert(key1.hash == 0x811c9dc5u);
  
  struct ht_key key2 = ht_const_str_key("");
  assert(key2.hash == 0x050c5d1fu);

  struct ht_key key3 = ht_const_str_key("foobar");
  assert(key3.hash == 0x0c1c9eb8u);
  
  struct ht_key key4 = ht_const_str_key("The rain in Spain\nfalls mainly on the plain.");
  assert(key4.hash == 0xb16a266au);
}

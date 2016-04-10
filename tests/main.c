#include <stdlib.h>


void
collision_test(void);

void
fnv1a_test(void);

void
full_table_test(void);

void
long_test(void);

void
string_test(void);

void
stringtable_test(void);

void
update_test(void);


int
main(void)
{
  collision_test();
  fnv1a_test();
  full_table_test();
  long_test();
  string_test();
  stringtable_test();
  update_test();
  
  return EXIT_SUCCESS;
}

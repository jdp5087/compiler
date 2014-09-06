#include <stdio.h>

#include "Main.h"
#include "cradle.h"
#include "asm_head.h"

int main()
{
  #ifdef HEADERS
  print_headers();
  #endif

  init();
  assignment();
  if (look != NEWLINE)
    _expected("Newline");

  #ifdef FOOTERS
  print_footers();
  #endif
  
  return 0;
}

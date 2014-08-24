#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

char* char_to_string(char c)
{
  char *s = (char*)malloc(sizeof(char)*2);
  sprintf(s, "%c", c);
  return s;
}

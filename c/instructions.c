#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include "instructions.h"
#include "cradle.h"
*/

/* Link this up with the rest write the rest of your
   instructions needed 
*/

char* base_instruction(void)
{
  char *r = (char*)malloc(4*sizeof(char));
  sprintf(r, "%s%c%s", "%s", TAB, "%s");
  return r;
}

char* two_arg_instruction(char* inst)
{
  char *b = base_instruction();
  char *registers = "%s,%s";
  size_t len = strlen(inst) + strlen(registers) + sizeof(char)*3;
  char *r = (char*)malloc(len);
  sprintf(r, b, inst, "%s,%s");
  free(b);
  return r;
}

char* movl(char *r1, char *r2)
{
  char *b = two_arg_instruction();
  size_t len = strlen(r1) + strlen(r2) + strlen("movl") + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, "movl", r1, r2);
  free(b);
  return r;
}

int main()
{
  char *s = movl("%IIIIIIIIIIIIIIIIIIIIeax","%ebx");
  printf("%s\n", s);
  free(s);
  return 0;
}

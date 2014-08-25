#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"
#include "helpers.h"
#include "instructions.h"

/* re-do the definition to take a struct instead of separate structures. */


typedef struct {
  const char *arr[] = {"+", "-"};
  int arr_len = 2;
}

const char *ops[] = {"+", "-"};
int ops_len = 2;

char look;

int in_cmp(char *target, const char *array[], int array_len)
{
  int i;
  for (i = 0; i < array_len; i++) {
    if (strcmp(target, array[i]) == 0)
      return 1;
  }
  return 0;
}

void _getchar(void)
{
  look = getchar();
}

void _error(char s[])
{
  printf("\n\aError: %s\n", s);
}

void _abort(char s[])
{
  _error(s);
  exit(1);
}

void _expected(char s[])
{
  char *t = strdup(s);
  strcat(t, " expected");
  _abort(t);
  free(t);
}

void match(char x)
{
  char *c = malloc(sizeof(char*)*4);
  char *d = "\"";
  char e[2];

  if (look == x) {
    _getchar();
  } else {
    sprintf(e, "%c", x);
    strcpy(c, d);
    strcat(c, e);
    strcat(c, d);
    _expected(c);
    free(c);
  }
}

char* getname(void)
{
  if (!isalpha(look)) {
    _expected("Name");
  } else {
    char *c = char_to_string(toupper(look));
    _getchar();
    return c;
  }
}

char* getnum(void)
{
  if (!isdigit(look)) {
    _expected("Digit");
  } else {
    char *c = char_to_string(look);
    _getchar();
    return c;
  }
}

void emit(char *s)
{
  printf("%c%s", TAB, s);
}

void emitln(char *s)
{
  emit(s);
  printf("\n");
}

void init(void)
{
  _getchar();
}

void term(void)
{
  char *c = (char*)malloc(sizeof(char)*3);
  sprintf(c, "%c%s", '$', getnum());
  char *s = movl(c, "%ebx");
  emitln(s);
  free(c);
  free(s);
}

void add(void)
{
  match('+');
  term();
  char *s = addl("%ecx", "%ebx");
  emitln(s);
  free(s);
}

void subtract(void)
{
  match('-');
  term();
  char *s = subl("%ecx", "%ebx");
  char *t = neg("%ebx");
  emitln(s);
  emitln(t);
  free(s);
  free(t);
}

void expression(void)
{
  term();
  char *s = movl("%ebx", "%ecx");
  emitln(s);
  free(s);
  switch (look){
  case '+' :
    add();
    break;
  case '-' :
    subtract();
    break;
  default :
    _expected("Addop");
  
  }

}


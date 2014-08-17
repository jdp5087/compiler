#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"

#define TAB '\t'

char look;

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

void match(char *x)
{
  char *c = (char*)malloc(128);
  char *d = "\"";
  
  if (look == *x) {
    _getchar();
  } else {
    strcpy(c, d);
    strcat(c, x);
    strcat(c, d);
    _expected(c);
    free(c);
  }
}

char getname(void)
{
  if (!isalpha(look)) {
    _expected("Name");
  } else {
    char c = toupper(look);
    _getchar();
    return c;
  }
}

char getnum(void)
{
  if (!isdigit(look)) {
    _expected("Digit");
  } else {
    char c = look;
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



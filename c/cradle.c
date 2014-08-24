#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"
#include "helpers.h"
#include "instructions.h"



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
  char *c = getnum();
  char *s = (char*)malloc(sizeof(char*)*14);
  strcpy(s, "movl\t");  
  strcat(s, c);
  strcat(s, ",%ebx");
  emitln(s);
  free(c);
  free(s);
}

void add(void)
{
  match('+');
  term();
  emitln("addl\t%ecx,%ebx");
}

void subtract(void)
{
  match('-');
  term();
  emitln("subl\t%ecx,%ebx");
  emitln("negl\t&ebx");
}

void expression(void)
{
  term();
  emitln("movl\t%ebx,ecx");
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


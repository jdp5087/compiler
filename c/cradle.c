#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"
#include "helpers.h"
#include "instructions.h"

/* re-do the definition to take a struct instead of separate structures. */

/* typedef struct { */
/*   const char *arr[] = {"+", "-"}; */
/*   int arr_len = 2; */
/* } stuff; */

const char *addops[] = {"+", "-"};
int addops_len = (sizeof(addops)/sizeof(char *));
const char *mulops[] = {"*", "/"};
int mulops_len = (sizeof(mulops)/sizeof(char *));

char look;

int in_cmp(char *target, const char *array[], int array_len)
{
  int i;
  for (i = 0; i < array_len; i = i+1) {
    if (strcmp(target, array[i]) == 0)
      return 1;
  }
  return 0;
}

int is_addop(char c)
{
  char *sc = char_to_string(c);
  int res = in_cmp(sc, addops, addops_len);
  free(sc);
  return res;
}

int is_mulop(char c)
{
  char *sc = char_to_string(c);
  int res = in_cmp(sc, mulops, mulops_len);
  free(sc);
  return res;
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

void factor(void)
{
  if (look == '(') {
    match('(');
    expression();
    match(')');
  } else {
    char *c = (char*)malloc(sizeof(char)*3);
    char *d = getnum();
    sprintf(c, "%c%s", '$', d);
    char *s = movl(c, "%eax");
    emitln(s);
    free(c);
    free(d);
    free(s);
  }
}

void multiply(void)
{

  match('*');
  factor();
  imul("(%esp)", "%eax");
  char *t = addl("$4", "%esp");
  emitln(t);
  free(t);
}

void divide(void)
{
  match('/');
  factor();
  char *r = movl("%eax", "%edx");
  char *s = xor("%eax", "%eax");
  char *t = idivl("(%esp)");  
  char *u = addl("$4", "%esp");
  emitln(r);
  emitln(s);
  emitln(t);
  emitln(u);
  free(r);
  free(s);
  free(t);
  free(u);
}

void term(void)
{
  char *s;
  factor();
  while (is_mulop(look)) {
    char *s = push("%eax");
    emitln(s);
    free(s);
    switch (look){
    case '*' :
      multiply();
      break;
    case '/' :
      divide();
      break;
    default :
      _expected("Mulop");
    }
  }
}

void add(void)
{
  match('+');
  term();
  char *s = addl("(%esp)", "%eax");
  char *r = addl("$4", "%esp");
  emitln(s);
  emitln(r);
  free(s);
  free(r);
}

void subtract(void)
{
  match('-');
  term();
  char *r = subl("(%esp)", "%eax");
  char *s = addl("$4", "%esp");
  char *t = neg("%eax");
  emitln(r);
  emitln(s);
  emitln(t);
  free(r);
  free(s);
  free(t);
}

void expression(void)
{
  char *s;
  if (is_addop(look)) {
    char *t = xor("%eax", "%eax");
    emitln(t);
    free(t);
  } else {
    term();
  }
  while (is_addop(look)) {
    char *s = push("%eax");
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
}



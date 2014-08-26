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
    movl(c, "%eax");
    free(c);
    free(d);
  }
}

void multiply(void)
{
  match('*');
  factor();
  imull("(%esp)", "%eax");
  addl("$4", "%esp");
}

void divide(void)
{
  match('/');
  factor();
  xor("%edx", "%edx");
  movl("%eax","%ebx");
  movl("(%esp)","%eax");
  addl("$4", "%esp");
  idivl("%ebx");
}

void term(void)
{
  factor();
  while (is_mulop(look)) {
    push("%eax");
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
  addl("(%esp)", "%eax");
  addl("$4", "%esp");
}

void subtract(void)
{
  match('-');
  term();
  subl("(%esp)", "%eax");
  addl("$4", "%esp");
  neg("%eax");
}

void expression(void)
{
  if (is_addop(look)) {
    xor("%eax", "%eax");
  } else {
    term();
  }
  while (is_addop(look)) {
    push("%eax");
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



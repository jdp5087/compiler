#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"
#include "helpers.h"
#include "instructions.h"


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


// Because function calls don't use immediate addressing,
// I'm going to leave the responsibility of formatting the $
// to the caller in getname, but not in getnum
char* getname(void)
{
  if (!isalpha(look)) {
    _expected("Name");
  } else {
    char *d = char_to_string(toupper(look));
    _getchar();
    return d;
  }
}

int getnum(void)
{
  int value = 0;
  if (!isdigit(look)) {
    _expected("Digit");
  }
  while (isdigit(look)) {
    value = (10*value) + (look - '0');
    _getchar();
  }
    return value; 
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

void ident(void)
{
  char *name = getname();
  if (look == '(') {
    match('(');
    match(')');
    call(name);
  } else {
    char *c = (char*)malloc(sizeof(char)*3);
    sprintf(c, "$%s", name);
    movl(c, "%eax");
    free(c);
  }
  free(name);
}

int factor(void)
{
  int result;
  if (look == '(') {
    match('(');
    result = expression();
    match(')');
    return result;
  } else {
    return getnum();
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
  xorl("%edx", "%edx");
  movl("%eax","%ebx");
  movl("(%esp)","%eax");
  addl("$4", "%esp");
  idivl("%ebx");
}

int term(void)
{
  int value = factor();
  while (is_mulop(look)) {
    switch (look){
    case '*' :
      match('*');
      value *= factor();
      break;
    case '/' :
      match('/');
      value /= factor();
      break;
    }
  }
  return value;
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
  negl("%eax");
}

int expression(void)
{
  int value;
  if (is_addop(look)) {
    value = 0;
  } else {
    value = term();
  }
  while (is_addop(look)) {
    switch (look) {
    case '+' :
      match('+');
      value += term();
      break;
    case '-' :
      match('-');
      value -= term();
      break;
    }
  }
  return value;
}

void assignment(void)
{
  char *name = getname();
  match('=');
  expression();
  lcomm(name, "1");
  movl("%eax", name);
  free(name);
}



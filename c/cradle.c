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

void skipwhite(void)
{
  while isspace(look) {
      _getchar();
    }
}

void _error(char s[])
{
  printf("\nError: %s\n", s);
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
  char *c = malloc(sizeof(char)*5);
  if (look == x) {
    _getchar();
    skipwhite();
  } else {
    sprintf(c, "\"%c\"", x);
    _expected(c);
    free(c);
  }
}


// Because function calls don't use immediate addressing,
// I'm going to leave the responsibility of formatting the $
// to the caller in getname, but not in getnum
char* getname(void)
{
  char *token = (char*)(malloc(sizeof(char)*TOKENSIZE));
  sprintf(token, "");
  if (!isalpha(look))
    _expected("Name");
  while (isalnum(look)) {
    sprintf(token, "%s%c", token, toupper(look));
    _getchar();
  }
  skipwhite();
  return token;
}

char* getnum(void)
{
  char *value = (char*)(malloc(sizeof(char)*TOKENSIZE));
  sprintf(value, "");
  if (!isdigit(look)) {
    _expected("Digit");
  }
  while (isdigit(look)) {
    sprintf(value, "%s%c", value, look);
    _getchar();
  }
  skipwhite();
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
  skipwhite();
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

void factor(void)
{
  if (look == '(') {
    match('(');
    expression();
    match(')');
  } else if (isalpha(look)) {
    ident();
  } else {
    char *num = getnum();
    movl(num, "%eax");
    free(num);
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

void term(void)
{
  factor();
  while (is_mulop(look)) {
    pushl("%eax");
    switch (look){
    case '*' :
      multiply();
      break;
    case '/' :
      divide();
      break;
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
  negl("%eax");
}

void expression(void)
{
  if (is_addop(look)) {
    xorl("%eax", "%eax");
  } else {
    term();
  }
  while (is_addop(look)) {
    pushl("%eax");
    switch (look){
    case '+' :
      add();
      break;
    case '-' :
      subtract();
      break;
    }
  }
}

void assignment(void)
{
  char *name = getname();
  match('=');
  expression();
  lcomm(name, "4");
  movl("%eax", name);
  free(name);
}



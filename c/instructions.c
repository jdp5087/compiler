#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#include "cradle.h"

char* base_instruction(void)
{
  char *r = (char*)malloc(4*sizeof(char));
  sprintf(r, "%s%c%s", "%s", TAB, "%s");
  return r;
}

char* one_arg_instruction(char* inst)
{
  char *b = base_instruction();
  char *registers = "%s";
  size_t len = strlen(inst) + strlen(registers) + sizeof(char)*2;
  char *r = (char*)malloc(len);
  sprintf(r, b, inst, registers);
  free(b);
  return r;
}

char* two_arg_instruction(char* inst)
{
  char *b = base_instruction();
  char *registers = "%s,%s";
  size_t len = strlen(inst) + strlen(registers) + sizeof(char)*3;
  char *r = (char*)malloc(len);
  sprintf(r, b, inst, registers);
  free(b);
  return r;
}

void movl(char *r1, char *r2)
{
  char *b = two_arg_instruction("movl");
  size_t len = strlen(b) + strlen(r1) + strlen(r2) + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1, r2);
  free(b);
  emitln(r);
  free(r);
}

void addl(char *r1, char *r2)
{
  char *b = two_arg_instruction("addl");
  size_t len = strlen(b) + strlen(r1) + strlen(r2) + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1, r2);
  free(b);
  emitln(r);
  free(r);
}

void subl(char *r1, char *r2)
{
  char *b = two_arg_instruction("subl");
  size_t len = strlen(b) + strlen(r1) + strlen(r2) + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1, r2);
  free(b);
  emitln(r);
  free(r);
}

void imull(char *r1, char *r2)
{
  char *b = two_arg_instruction("imul");
  size_t len = strlen(b) + strlen(r1) + strlen(r2) + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1, r2);
  free(b);
  emitln(r);
  free(r);
}

void idivl(char *r1)
{
  char *b = one_arg_instruction("idivl");
  size_t len = strlen(b) + strlen(r1) + sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1);
  free(b);
  emitln(r);
  free(r);
}

void negl(char *r1)
{
  char *b = one_arg_instruction("neg");
  size_t len = strlen(b) + strlen(r1) + sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1);
  free(b);
  emitln(r);
  free(r);
}

void pushl(char *r1)
{
  char *b = one_arg_instruction("push");
  size_t len = strlen(b) + strlen(r1) + sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1);
  free(b);
  emitln(r);
  free(r);
}

void xorl(char *r1, char *r2)
{
  char *b = two_arg_instruction("xor");
  size_t len = strlen(b) + strlen(r1) + strlen(r2) + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, r1, r2);
  free(b);
  emitln(r);
  free(r);
}

void call(char *symbol)
{
  char *b = one_arg_instruction("call");
  size_t len = strlen(b) + strlen(symbol) + sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, symbol);
  free(b);
  emitln(r);
  free(r);
}

void lcomm(char *symbol, char *bytes)
{
  char *b = two_arg_instruction(".lcomm");
  size_t len = strlen(b) + strlen(symbol) + strlen(bytes) + 3*sizeof(char);
  char *r = (char*)malloc(len);
  sprintf(r, b, symbol, bytes);
  free(b);
  emitln(r);
  free(r);
}


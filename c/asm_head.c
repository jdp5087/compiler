#include <stdio.h>

void print_headers(void)
{
  printf(".section\t.bss\n\n");
  printf(".section\t.data\n\n");
  printf(".section\t.text\n\n");
  printf(".globl\t_start\n\n");
  printf("_start:\n");
  return;
}

void print_footers(void)
{
  printf("\tmovl\t%%eax, %%ebx\n");
  printf("\tmovl\t$1, %%eax\n");
  printf("\tint\t$0x80\n");
  return;
}

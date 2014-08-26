char* base_instruction(void);
char* one_arg_instruction(char* inst);
char* two_arg_instruction(char* inst);

void movl(char *r1, char *r2);
void addl(char *r1, char *r2);
void subl(char *r1, char *r2);
void imull(char *r1, char *r2);
void idivl(char *r1);
void negl(char *r1);
void pushl(char *r1);
void xorl(char *r1, char *r2);

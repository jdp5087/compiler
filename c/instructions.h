char* base_instruction(void);
char* one_arg_instruction(char* inst);
char* two_arg_instruction(char* inst);

char* movl(char *r1, char *r2);
char* addl(char *r1, char *r2);
char* subl(char *r1, char *r2);
void imul(char *r1, char *r2);
char* idivl(char *r1);
char* neg(char *r1);
char* push(char *r1);
char* xor(char *r1, char *r2);

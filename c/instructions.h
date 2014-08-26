char* base_instruction(void);
char* one_arg_instruction(char* inst);
char* two_arg_instruction(char* inst);

void movl(char *r1, char *r2);
void addl(char *r1, char *r2);
void subl(char *r1, char *r2);
void imul(char *r1, char *r2);
void idivl(char *r1);
void neg(char *r1);
void push(char *r1);
void xor(char *r1, char *r2);

extern char look;

void _getchar(void);
void _error(char s[]);
void _abort(char s[]);
void _expected(char s[]);
void match(char x);
char* getname(void);
char* getnum(void);
void emit(char *s);
void emitln(char *s);
void init(void);

#define TAB '\t'

extern char look;
extern const char *addops[];
extern int addops_len;
extern const char *mulops[];
extern int mulops_len;

int in_cmp(char *, const char *[], int);
void _getchar(void);
void _error(char []);
void _abort(char []);
void _expected(char []);
void match(char);
char* getname(void);
char* getnum(void);
void emit(char *);
void emitln(char *);
void init(void);
void expression(void);

#define TAB '\t'
#define NEWLINE '\n'

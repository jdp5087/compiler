import sys
import os


_IN = sys.stdin.read
_OUT = sys.stdout.write

TAB = '\t'
LOOK = ''

def getchar():
    global LOOK
    LOOK = _IN(1)

def error(s):
    _OUT('{}Error: {}.{}'.format(os.linesep, s, os.linesep))

def abort(s):
    error(s)
    exit(1)

def expected(s):
    abort('{} expected'.format(s))

def match(x):
    if LOOK == x:
        getchar()
    else:
        expected('''"{}"'''.format(x))

def isalpha(c):
    return c.isalpha()

def isdigit(c):
    return c.isdigit()

def getname():
    if not isalpha(LOOK):
        expected('Name')
    c = LOOK[:].upper()
    getchar()
    return c

def getnum():
    if not isdigit(LOOK):
        expected('Integer')
    c = LOOK[:]
    getchar()
    return c

def emit(s):
    _OUT('{}{}'.format(TAB, s))

def emitln(s):
    emit('{}'.format(s))
    _OUT(os.linesep)
    
def init():
    getchar()

def term():
    emitln(movl('$' + getnum(),'%eax'))

def expression():
    term()
    emitln(movl('%eax','%ebx'))
    try:
        op = OPS[LOOK]
        op()
    except KeyError:
        expected('Addop')

def add():
    match('+')
    term()
    emitln(addl('%eax','%ebx'))

def subtract():
    match('-')
    term()
    emitln(subl('%eax','%ebx'))

OPS = {
    '+': add,
    '-': subtract,
    }

def main():
    init()
    expression()

def movl(v1, v2):
    return asm_inst('movl').format(v1,v2)

def addl(v1, v2):
    return asm_inst('addl').format(v1,v2)

def subl(v1,v2):
    return asm_inst('subl').format(v1,v2)

def asm_inst(inst):
    format_dict = {'inst': inst,
                   'TAB': TAB,
                   'registers': '{0},{1}'}
    return '{inst}{TAB}{registers}'.format(**format_dict)
                                     
                                     
if __name__ == '__main__':
    main()
    
    
    

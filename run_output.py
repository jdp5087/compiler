import subprocess

command = "((echo 1+1) | (./df))"

def parse(cmd):
    if not (cmd.startswith('(') and cmd.endswith(')')):
        return cmd
    l = []
    cmd = cmd[1:-1]
    while len(cmd) > 0:
        if cmd.startswith('('):
            res = cmd.find(')')
            if res == -1:
                raise ValueError('Mismatched parenthesis')
            l.append(parse(cmd[0:res+1]))
            try:
                cmd = cmd[res+2:]
            except IndexError:
                cmd = cmd[res+1:]
        else:
            find_res = cmd.find(' ')
            res = find_res if find_res != -1 else (len(cmd)-1)
            l.append(parse(cmd[:res]))
            try:
                cmd = cmd[res+1:]
            except IndexError:
                cmd = cmd[res:]
    return l

result = parse(command)


import sys

print(subprocess.PIPE.to_bytes(65))
print(subprocess.call(['./df'], stdin=subprocess.PIPE))
sys.stdin.write("1+2")

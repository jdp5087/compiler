cradle.o: cradle.c cradle.h helpers.h
	gcc -c cradle.c

Main.o: Main.c Main.h cradle.h
	gcc -c Main.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

instructions.o: instructions.c instructions.h
	gcc -c instructions.c

asm_head.o: asm_head.c asm_head.h
	gcc -c asm_head.c

df: cradle.o Main.o helpers.o instructions.o asm_head.o
	gcc cradle.o Main.o helpers.o instructions.o asm_head.o -o ../df

cradle.o: cradle.c cradle.h helpers.h
	gcc -c cradle.c

Main.o: Main.c cradle.h
	gcc -c Main.c

helpers.o: helpers.c helpers.h
	gcc -c helpers.c

instructions.o: instructions.c instructions.h
	gcc -c instructions.c

df: cradle.o Main.o helpers.o instructions.o
	gcc cradle.o Main.o helpers.o instructions.o -o ../df

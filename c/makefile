cradle.o: cradle.c cradle.h
	gcc -c cradle.c

Main.o: Main.c cradle.h
	gcc -c Main.c

df: cradle.o Main.o
	gcc cradle.o Main.o -o df

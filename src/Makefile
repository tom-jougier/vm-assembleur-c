all: simulateur

simulateur: assembleur.o processeur.o main.o
	gcc -o simulateur assembleur.o processeur.o main.o

assembleur.o: assembleur.c
	gcc -o assembleur.o -c assembleur.c -Wall

processeur.o: processeur.c
	gcc -o processeur.o -c processeur.c -Wall

main.o: main.c assembleur.h processeur.h
	gcc -o main.o -c main.c -Wall

clean:
	rm -rf *.o simulateur

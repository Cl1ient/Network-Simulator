all:
	gcc -c main.c


	gcc -c src/adresse.c
	gcc -c src/algos.c
	gcc -c src/config.c
	gcc -c src/graphe.c
	gcc -c src/reseau.c
	gcc -c src/station.c
	gcc -c src/switch.c
	gcc -c src/trame.c
	gcc -c src/utils.c
	gcc -o programme adresse.o algos.o config.o graphe.o main.o reseau.o station.o stp.o switch.o trame.o utils.o

clean:
	rm -f *.o programme

all:
	gcc -g -c main.c


	gcc -g -c src/adresse.c
	gcc -g -c src/algos.c
	gcc -g -c src/config.c
	gcc -g -c src/graphe.c
	gcc -g -c src/reseau.c
	gcc -g -c src/station.c
	gcc -g -c src/switch.c
	gcc -g -c src/utils.c
	gcc -g -c src/tram.c
	gcc -o programme adresse.o algos.o config.o graphe.o main.o reseau.o station.o switch.o utils.o tram.o 

clean:
	rm -f *.o programme


valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./programme
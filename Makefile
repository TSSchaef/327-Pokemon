#Makefile

poke: poke.o io.o character.o heap.o pokeClasses.o pokemon.o
	g++ poke.o character.o heap.o io.o pokeClasses.o pokemon.o -o poke -lncurses

pokemon.o: src/pokemon.cpp src/pokemon.h src/pokeClasses.h
	g++ -Wall -Werror -g src/pokemon.cpp -c

heap.o: src/heap.c src/heap.h
	gcc -Wall -Werror -g src/heap.c -c

character.o: src/character.cpp src/character.h src/pokemon.h
	g++ -Wall -Werror -g src/character.cpp -c

pokeClasses.o: src/pokeClasses.cpp src/pokeClasses.h
	g++ -Wall -Werror -g src/pokeClasses.cpp -c

io.o: pokeClasses.o src/io.cpp
	g++ -Wall -Werror -g src/io.cpp -c

poke.o: src/poke.cpp src/heap.h src/character.h src/pokeClasses.h src/io.h
	g++ -Wall -Werror -g src/poke.cpp -c

.PHONY: clean

clean:
	rm -f *.o debug.txt stack* core* HW* poke *.log valgrind*

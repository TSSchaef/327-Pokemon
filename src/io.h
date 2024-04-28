#ifndef IO_H
#define IO_H

/*
 *	Finds pokedex DB and scans raw information for pokemon into
 *	vectors of pokeClasses. Vectors will be used to generate actual 
 *	pokemon for the game.
 *
 *	Output is actually handled in poke.cpp.
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <climits>
#include <sys/stat.h>

#include "pokeClasses.h"

std::string *findDB();
std::vector<poke_class *> scanPokemon(std::string path);
std::vector<poke_class *> scanMoves(std::string path);
std::vector<poke_class *> scanPokeMoves(std::string path);
std::vector<poke_class *> scanPokeSpecies(std::string path);
std::vector<poke_class *> scanExperience(std::string path);
std::vector<poke_class *> scanTypeNames(std::string path);
std::vector<poke_class *> scanPokeStats(std::string path);
std::vector<poke_class *> scanStats(std::string path);
std::vector<poke_class *> scanPokeTypes(std::string path);

#endif

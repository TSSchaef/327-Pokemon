#ifndef CHARACTER_H
#define CHARACTER_H

#include "heap.h"
#include "pokemon.h"
#include <vector>

#define WIDTH 80
#define HEIGHT 21
#define WORLD_SIZE 401

typedef enum charType{
	PC,
	Hiker,
	Rival,
	Pacer,
	Wanderer,
	Sentry,
	Explorer
} charType_t;

typedef enum dim{
	dim_x,
	dim_y
} dim_t;

typedef enum direction{
	North,
	South,
	East,
	West
} direction_t;

typedef class character{
	public:
	heap_node_t *hn;
	charType_t type;
	int pos[2];
	int cost;
	int defeated;
	direction_t dir;
	std::vector<pokemon *> pets;
} character_t;

class bag{
public:
	int revives, potions, pokeballs;
};

typedef class path {
	public:
	heap_node_t *hn;
	uint8_t pos[2];
	int32_t cost;
} path_t;

typedef class scene scene_t;

extern int xCord, yCord, numTrainers;
extern scene_t *world[WORLD_SIZE][WORLD_SIZE];
extern bag pcBag;
extern std::vector<pokemon *> pcPokemon;
extern unsigned int currPoke;

int moveCharacter(scene_t *s, character_t *c);
int movePlayer(scene_t *s, character_t *c);
int fly();
int showTrainers(scene_t *s);
int enterMarket();
int enterCenter();
void initScreen();
const char *getType(unsigned int type);
void battle(character_t *c);
void encounter(pokemon *p);
int costOf(int x_dim, int y_dim, charType_t type);
void dijkstra(path_t path[WIDTH][HEIGHT], charType_t type);
int openBag(bool inEncounter, bool inBattle);
bool pcDefeated();

#endif

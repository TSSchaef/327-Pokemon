#ifndef POKE_H
#define POKE_H

#include "pokeClasses.h"
#include "io.h"
#include "heap.h"
#include "character.h"

#define ORIGIN 200

//% chance for given terrain to spread 
//on a given iteration of terrain generation 
//(num/100)
#define CHNC_ROCK_SPREAD 11
#define CHNC_WATER_SPREAD 14
#define CHNC_TREE_SPREAD 11
#define CHNC_WEEDS_SPREAD 17
#define CHNC_GRASS_SPREAD 20

//% chance for buildings to spawn near the center
//and edges (num/100) - flat edge chance used beyond
//200 units from center (Manhattan distance)
#define CHNC_BLDNG_CENTER 50
#define CHNC_BLDNG_EDGE 5

//for defining chances out of 100
#define CHNC 100

#define CHNC_FLEE 40
#define CHNC_CAPTURE 60

#define COLOR_ROCK 1
#define COLOR_TREE 2
#define COLOR_WATER 3
#define COLOR_PATH 4
#define COLOR_SGRASS 5
#define COLOR_TGRASS 6
#define COLOR_BUILDING 7
#define COLOR_TRAINER 8
#define COLOR_PC 9
#define COLOR_DEFAULT 10

typedef class character character_t;

typedef class scene{
	public:
	heap_t eventDriver;
	char scene[WIDTH][HEIGHT];
	character_t *charScene[WIDTH][HEIGHT];
	//gates
	int n, s, e, w;
	//cords in world
	int x, y;
} scene_t;

extern path_t hikerMap[WIDTH][HEIGHT];
extern path_t rivalMap[WIDTH][HEIGHT];
extern int player[2];

void printScene(scene_t *s);
void fillBorder(scene_t *s);
int getSeed();
void generateTerrain(scene_t *s);
void generatePaths(scene_t *s);
void placeBuildings(scene_t *s);
void buildScene(scene_t *s, int x, int y);
void clearScene(scene_t *s);
void startWorld();
void endWorld();
void switchScene(int x, int y);
void findPC(scene_t *s, int x, int y);
void placePlayer(scene_t *s, int x, int y);
void placeNPCs(int numNPCs, scene_t *s);

#endif

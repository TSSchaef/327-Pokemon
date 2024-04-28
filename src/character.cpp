#include <cstdlib>
#include <climits>
#include <ncurses.h>
#include <unistd.h>

#include "character.h"
#include "pokemon.h"
#include "poke.h"

path_t hikerMap[WIDTH][HEIGHT];
path_t rivalMap[WIDTH][HEIGHT];
int player[2];

static int32_t path_cmp(const void *key, const void *with) {
  return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

//give the cost to travel to a given square
int costOf(int x_dim, int y_dim, charType_t type){
	switch(type)
	{
		case Hiker:
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '%' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '^' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == ':'){
		       		return 15;
			}
	
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '#' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '.'){
		       		return 10;
			}
			
			if(world[xCord][yCord]->scene[x_dim][y_dim] == 'C' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == 'M'){
		       		return 50;
			}
			
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '~'){
				return INT_MAX;
			}
			break;
	
		case PC:
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '%' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '^' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '~'){
		       		return INT_MAX;
			}
	
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '#' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '.' || 
				world[xCord][yCord]->scene[x_dim][y_dim] == 'C' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == 'M') {
				return 10;
			}
			
			if(world[xCord][yCord]->scene[x_dim][y_dim] == ':'){
				return 20;
			}
			break;
		default:
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '%' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '^' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '~'){
		       		return INT_MAX;
			}
	
			if(world[xCord][yCord]->scene[x_dim][y_dim] == '#' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == '.'){
		       		return 10;
			}
			
			if(world[xCord][yCord]->scene[x_dim][y_dim] == 'C' ||
				world[xCord][yCord]->scene[x_dim][y_dim] == 'M') {
				return 50;
			}
			
			if(world[xCord][yCord]->scene[x_dim][y_dim] == ':'){
				return 20;
			}
			break;
	}
	return INT_MAX;
}

//Used to determine pathfinding for NPC's
void dijkstra(path_t path[WIDTH][HEIGHT], charType_t type){
  static path_t *p;
  heap_t h;
  uint32_t x, y;

	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			path[x][y].pos[dim_y] = y;
			path[x][y].pos[dim_x] = x;
		}
	}
  
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			path[x][y].cost = INT_MAX;
		}
	}

	path[player[dim_x]][player[dim_y]].cost = 0;

	heap_init(&h, path_cmp, NULL);

	for (y = 1; y < HEIGHT - 1; y++) {
		for (x = 1; x < WIDTH - 1; x++) {
			if(costOf(x, y, type) != INT_MAX){
				path[x][y].hn = heap_insert(&h, &path[x][y]);
			}else{
				path[x][y].hn = NULL;
			}
		}
	}

	while ((p = (path_t *) heap_remove_min(&h))) {
		p->hn = NULL;

		if ((path[p->pos[dim_x] - 1][p->pos[dim_y]    ].hn) &&
			(path[p->pos[dim_x] - 1][p->pos[dim_y]    ].cost >
			(p->cost + costOf(p->pos[dim_x] - 1, p->pos[dim_y]    , type)))) {
				path[p->pos[dim_x] - 1][p->pos[dim_y]    ].cost =
				(p->cost + costOf(p->pos[dim_x] - 1,p->pos[dim_y]    , type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x] - 1][p->pos[dim_y]    ].hn);
		 }

		if ((path[p->pos[dim_x] + 1][p->pos[dim_y]    ].hn) &&
			(path[p->pos[dim_x] + 1][p->pos[dim_y]    ].cost >
			(p->cost + costOf(p->pos[dim_x] + 1,p->pos[dim_y]    , type)))) {
				path[p->pos[dim_x] + 1][p->pos[dim_y]    ].cost =
				(p->cost + costOf(p->pos[dim_x] + 1,p->pos[dim_y]    , type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x] + 1][p->pos[dim_y]    ].hn);
		 }

		if ((path[p->pos[dim_x]    ][p->pos[dim_y] - 1].hn) &&
			(path[p->pos[dim_x]    ][p->pos[dim_y] - 1].cost >
			(p->cost + costOf(p->pos[dim_x]    ,p->pos[dim_y] - 1, type)))) {
				path[p->pos[dim_x]    ][p->pos[dim_y] - 1].cost =
				(p->cost + costOf(p->pos[dim_x]    ,p->pos[dim_y] - 1, type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x]    ][p->pos[dim_y] - 1].hn);
		 }

		if ((path[p->pos[dim_x]    ][p->pos[dim_y] + 1].hn) &&
			(path[p->pos[dim_x]    ][p->pos[dim_y] + 1].cost >
			(p->cost + costOf(p->pos[dim_x]    ,p->pos[dim_y] + 1, type)))) {
				path[p->pos[dim_x]    ][p->pos[dim_y] + 1].cost =
				(p->cost + costOf(p->pos[dim_x]    ,p->pos[dim_y] + 1, type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x]    ][p->pos[dim_y] + 1].hn);
		}
		 
		if ((path[p->pos[dim_x] - 1][p->pos[dim_y] - 1].hn) &&
			(path[p->pos[dim_x] - 1][p->pos[dim_y] - 1].cost >
			(p->cost + costOf(p->pos[dim_x] - 1,p->pos[dim_y] - 1, type)))) {
				path[p->pos[dim_x] - 1][p->pos[dim_y] - 1].cost =
				(p->cost + costOf(p->pos[dim_x] - 1,p->pos[dim_y] - 1, type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x] - 1][p->pos[dim_y] - 1].hn);
		 }

		if ((path[p->pos[dim_x] + 1][p->pos[dim_y] - 1].hn) &&
			(path[p->pos[dim_x] + 1][p->pos[dim_y] - 1].cost >
			(p->cost + costOf(p->pos[dim_x] + 1,p->pos[dim_y] - 1, type)))) {
				path[p->pos[dim_x] + 1][p->pos[dim_y] - 1].cost =
				(p->cost + costOf(p->pos[dim_x] + 1, p->pos[dim_y] - 1, type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x] + 1][p->pos[dim_y] - 1].hn);
		 }

		if ((path[p->pos[dim_x] - 1][p->pos[dim_y] + 1].hn) &&
			(path[p->pos[dim_x] - 1][p->pos[dim_y] + 1].cost >
			(p->cost + costOf(p->pos[dim_x] - 1, p->pos[dim_y] + 1, type)))) {
				path[p->pos[dim_x] - 1][p->pos[dim_y] + 1].cost =
				(p->cost + costOf(p->pos[dim_x] - 1,p->pos[dim_y] + 1, type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x] - 1][p->pos[dim_y] + 1].hn);
		 }

		if ((path[p->pos[dim_x] + 1][p->pos[dim_y] + 1].hn) &&
			(path[p->pos[dim_x] + 1][p->pos[dim_y] + 1].cost >
			(p->cost + costOf(p->pos[dim_x] + 1,p->pos[dim_y] + 1, type)))) {
				path[p->pos[dim_x] + 1][p->pos[dim_y] + 1].cost =
				(p->cost + costOf(p->pos[dim_x] + 1,p->pos[dim_y] + 1, type));
				 heap_decrease_key_no_replace(&h, 
					 path[p->pos[dim_x] + 1][p->pos[dim_y] + 1].hn);
		 }
	}
	heap_delete(&h);
	//clean negative numbers from unreachable terrain
	for (y = 1; y < HEIGHT - 1; y++) {
		for (x = 1; x < WIDTH - 1; x++) {
			if(path[x][y].cost < 0){
				path[x][y].cost = INT_MAX;
			}
		}
	}
}


int moveCharacter(scene_t *s, character_t *c){
	int x, y;	
	int maxGradient = INT_MAX; 
	int dirX, dirY;

	x = c->pos[dim_x];
	y = c->pos[dim_y];
	
	switch(c->type){
		case PC:
			printScene(world[xCord][yCord]);
			return movePlayer(world[xCord][yCord], c);
			printScene(world[xCord][yCord]);
		case Hiker:		
			if(hikerMap[x][y - 1].cost < maxGradient &&
					s->charScene[x][y - 1] == NULL){
			       	maxGradient = hikerMap[x][y - 1].cost;
				dirX = x;
				dirY = y - 1;
			}
			if(hikerMap[x][y + 1].cost < maxGradient &&
					s->charScene[x][y + 1] == NULL){
			       	maxGradient = hikerMap[x][y + 1].cost;
				dirX = x;
				dirY = y + 1;
			}
			if(hikerMap[x - 1][y].cost < maxGradient &&
					s->charScene[x - 1][y] == NULL){
			       	maxGradient = hikerMap[x - 1][y].cost;
				dirX = x - 1;
				dirY = y;
			}
			if(hikerMap[x + 1][y].cost < maxGradient &&
					s->charScene[x + 1][y] == NULL){
			       	maxGradient = hikerMap[x + 1][y].cost;
				dirX = x + 1;
				dirY = y;
			}
			if(hikerMap[x - 1][y - 1].cost < maxGradient &&
					s->charScene[x - 1][y - 1] == NULL){
			       	maxGradient = hikerMap[x - 1][y - 1].cost;
				dirX = x - 1;
				dirY = y - 1;
			}
			if(hikerMap[x - 1][y + 1].cost < maxGradient &&
					s->charScene[x - 1][y + 1] == NULL){
			       	maxGradient = hikerMap[x - 1][y + 1].cost;
				dirX = x - 1;
				dirY = y + 1;
			}
			if(hikerMap[x + 1][y - 1].cost < maxGradient &&
					s->charScene[x + 1][y - 1] == NULL){
			       	maxGradient = hikerMap[x + 1][y - 1].cost;
				dirX = x + 1;
				dirY = y - 1;
			}
			if(hikerMap[x + 1][y + 1].cost < maxGradient &&
					s->charScene[x + 1][y + 1] == NULL){
			       	maxGradient = hikerMap[x + 1][y + 1].cost;
				dirX = x + 1;
				dirY = y + 1;
			}
			
			if(maxGradient == INT_MAX){
				c->cost += costOf(x, y, Hiker);
			}else if(c->defeated){
				switch(c->dir){
					case North:
						if(costOf(x, y - 1, Hiker) != INT_MAX &&
								s->charScene[x][y - 1] == NULL &&
								c->pos[dim_y] > 1){
							c->pos[dim_y] -= 1;
							s->charScene[x][y] = NULL;
							s->charScene[x][y - 1] = c;
							c->cost += costOf(x, y - 1, Hiker);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Hiker);
						}
						break;
					case South:
						if(costOf(x, y + 1, Hiker) != INT_MAX &&
								s->charScene[x][y + 1] == NULL &&
								c->pos[dim_y] < HEIGHT - 2){
							c->pos[dim_y] += 1;
							s->charScene[x][y] = NULL;
							s->charScene[x][y + 1] = c;
							c->cost += costOf(x, y + 1, Hiker);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Hiker);
						}
						break;
					case East:
						if(costOf(x + 1, y, Hiker) != INT_MAX && 
								s->charScene[x + 1][y] == NULL &&
								c->pos[dim_x] < WIDTH - 2){
							c->pos[dim_x] += 1;
							s->charScene[x][y] = NULL;
							s->charScene[x + 1][y] = c;
							c->cost += costOf(x + 1, y, Hiker);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Hiker);
						}
						break;
					case West:
						if(costOf(x - 1, y, Hiker) != INT_MAX &&
								s->charScene[x - 1][y] == NULL &&
								c->pos[dim_x] > 1){
							c->pos[dim_x] -= 1;
							s->charScene[x][y] = NULL;
							s->charScene[x - 1][y] = c;
							c->cost += costOf(x - 1, y, Hiker);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Hiker);
						}
						break;
				}
				return 1;
			}else if((x + 1 == player[dim_x] && y + 1 == player[dim_y]) ||
					(x + 1 == player[dim_x] && y     == player[dim_y]) ||
					(x     == player[dim_x] && y + 1 == player[dim_y]) ||
					(x + 1 == player[dim_x] && y - 1 == player[dim_y]) ||
					(x - 1 == player[dim_x] && y + 1 == player[dim_y]) ||
					(x     == player[dim_x] && y - 1 == player[dim_y]) ||
					(x - 1 == player[dim_x] && y     == player[dim_y]) ||
					(x - 1 == player[dim_x] && y - 1 == player[dim_y])){
				battle(c);
			}else{
				c->cost += costOf(dirX, dirY, Hiker);
				s->charScene[dirX][dirY] = c;
				s->charScene[x][y] = NULL;
				c->pos[dim_x] = dirX;
				c->pos[dim_y] = dirY;
			}
			return 1;
		case Rival:
			if(rivalMap[x][y - 1].cost < maxGradient &&
					s->charScene[x][y - 1] == NULL){
			       	maxGradient = rivalMap[x][y - 1].cost;
				dirX = x;
				dirY = y - 1;
			}
			if(rivalMap[x][y + 1].cost < maxGradient &&
					s->charScene[x][y + 1] == NULL){
			       	maxGradient = rivalMap[x][y + 1].cost;
				dirX = x;
				dirY = y + 1;
			}
			if(rivalMap[x - 1][y].cost < maxGradient &&
					s->charScene[x - 1][y] == NULL){
			       	maxGradient = rivalMap[x - 1][y].cost;
				dirX = x - 1;
				dirY = y;
			}
			if(rivalMap[x + 1][y].cost < maxGradient &&
					s->charScene[x + 1][y] == NULL){
			       	maxGradient = rivalMap[x + 1][y].cost;
				dirX = x + 1;
				dirY = y;
			}
			if(rivalMap[x - 1][y - 1].cost < maxGradient &&
					s->charScene[x - 1][y - 1] == NULL){
			       	maxGradient = rivalMap[x - 1][y - 1].cost;
				dirX = x - 1;
				dirY = y - 1;
			}
			if(rivalMap[x - 1][y + 1].cost < maxGradient &&
					s->charScene[x - 1][y + 1] == NULL){
			       	maxGradient = rivalMap[x - 1][y + 1].cost;
				dirX = x - 1;
				dirY = y + 1;
			}
			if(rivalMap[x + 1][y - 1].cost < maxGradient &&
					s->charScene[x + 1][y - 1] == NULL){
			       	maxGradient = rivalMap[x + 1][y - 1].cost;
				dirX = x + 1;
				dirY = y - 1;
			}
			if(rivalMap[x + 1][y + 1].cost < maxGradient &&
					s->charScene[x + 1][y + 1] == NULL){
			       	maxGradient = rivalMap[x + 1][y + 1].cost;
				dirX = x + 1;
				dirY = y + 1;
			}
			
			if(maxGradient == INT_MAX){
				c->cost += costOf(x, y, Rival);
			}else if(c->defeated){
				switch(c->dir){
					case North:
						if(costOf(x, y - 1, Rival) != INT_MAX &&
								s->charScene[x][y - 1] == NULL &&
								c->pos[dim_y] > 1){
							c->pos[dim_y] -= 1;
							s->charScene[x][y] = NULL;
							s->charScene[x][y - 1] = c;
							c->cost += costOf(x, y - 1, Rival);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Rival);
						}
						break;
					case South:
						if(costOf(x, y + 1, Rival) != INT_MAX &&
								s->charScene[x][y + 1] == NULL &&
								c->pos[dim_y] < HEIGHT - 2){
							c->pos[dim_y] += 1;
							s->charScene[x][y] = NULL;
							s->charScene[x][y + 1] = c;
							c->cost += costOf(x, y + 1, Rival);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Rival);
						}
						break;
					case East:
						if(costOf(x + 1, y, Rival) != INT_MAX && 
								s->charScene[x + 1][y] == NULL &&
								c->pos[dim_x] < WIDTH - 2){
							c->pos[dim_x] += 1;
							s->charScene[x][y] = NULL;
							s->charScene[x + 1][y] = c;
							c->cost += costOf(x + 1, y, Rival);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Rival);
						}
						break;
					case West:
						if(costOf(x - 1, y, Rival) != INT_MAX &&
								s->charScene[x - 1][y] == NULL &&
								c->pos[dim_x] > 1){
							c->pos[dim_x] -= 1;
							s->charScene[x][y] = NULL;
							s->charScene[x - 1][y] = c;
							c->cost += costOf(x - 1, y, Rival);
						}else{
							c->dir = static_cast<direction_t>(rand() % 4);
							c->cost += costOf(x, y, Rival);
						}
						break;
				}
				return 1;
			}else if((x + 1 == player[dim_x] && y + 1 == player[dim_y]) ||
					(x + 1 == player[dim_x] && y     == player[dim_y]) ||
					(x     == player[dim_x] && y + 1 == player[dim_y]) ||
					(x + 1 == player[dim_x] && y - 1 == player[dim_y]) ||
					(x - 1 == player[dim_x] && y + 1 == player[dim_y]) ||
					(x     == player[dim_x] && y - 1 == player[dim_y]) ||
					(x - 1 == player[dim_x] && y     == player[dim_y]) ||
					(x - 1 == player[dim_x] && y - 1 == player[dim_y])){
				battle(c);
			}else{
				c->cost += costOf(dirX, dirY, Rival);
				s->charScene[dirX][dirY] = c;
				s->charScene[x][y] = NULL;
				c->pos[dim_x] = dirX;
				c->pos[dim_y] = dirY;
			}
			return 1;
		case Pacer:
			switch(c->dir){
				case North:
					if(costOf(x, y - 1, Pacer) != INT_MAX &&
							s->charScene[x][y - 1] == NULL &&
							c->pos[dim_y] > 1){
						c->pos[dim_y] -= 1;
						s->charScene[x][y] = NULL;
						s->charScene[x][y - 1] = c;
						c->cost += costOf(x, y - 1, Pacer);
					}else{
						c->dir = South;
						c->cost += costOf(x, y, Pacer);
					}
					break;
				case South:
					if(costOf(x, y + 1, Pacer) != INT_MAX &&
							s->charScene[x][y + 1] == NULL &&
							c->pos[dim_y] < HEIGHT - 2){
						c->pos[dim_y] += 1;
						s->charScene[x][y] = NULL;
						s->charScene[x][y + 1] = c;
						c->cost += costOf(x, y + 1, Pacer);
					}else{
						c->dir = North;
						c->cost += costOf(x, y, Pacer);
					}
					break;
				case East:
					if(costOf(x + 1, y, Pacer) != INT_MAX && 
							s->charScene[x + 1][y] == NULL &&
							c->pos[dim_x] < WIDTH - 2){
						c->pos[dim_x] += 1;
						s->charScene[x][y] = NULL;
						s->charScene[x + 1][y] = c;
						c->cost += costOf(x + 1, y, Pacer);
					}else{
						c->dir = West;
						c->cost += costOf(x, y, Pacer);
					}
					break;
				case West:
					if(costOf(x - 1, y, Pacer) != INT_MAX &&
							s->charScene[x - 1][y] == NULL &&
							c->pos[dim_x] > 1){
						c->pos[dim_x] -= 1;
						s->charScene[x][y] = NULL;
						s->charScene[x - 1][y] = c;
						c->cost += costOf(x - 1, y, Pacer);
					}else{
						c->dir = East;
						c->cost += costOf(x, y, Pacer);
					}
					break;
			}
			return 1;
		case Wanderer:
			switch(c->dir){
				case North:
					if(s->scene[x][y] == s->scene[x][y - 1] &&
							s->charScene[x][y - 1] == NULL &&
							c->pos[dim_y] > 1){
						c->pos[dim_y] -= 1;
						s->charScene[x][y] = NULL;
						s->charScene[x][y - 1] = c;
						c->cost += costOf(x, y - 1, Wanderer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Wanderer);
					}
					break;
				case South:
					if(s->scene[x][y] == s->scene[x][y + 1] &&
							s->charScene[x][y + 1] == NULL &&
							c->pos[dim_y] < HEIGHT - 2){
						c->pos[dim_y] += 1;
						s->charScene[x][y] = NULL;
						s->charScene[x][y + 1] = c;
						c->cost += costOf(x, y + 1, Wanderer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Wanderer);
					}
					break;
				case East:
					if(s->scene[x][y] == s->scene[x + 1][y] && 
							s->charScene[x + 1][y] == NULL &&
							c->pos[dim_x] < WIDTH - 2){
						c->pos[dim_x] += 1;
						s->charScene[x][y] = NULL;
						s->charScene[x + 1][y] = c;
						c->cost += costOf(x + 1, y, Wanderer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Wanderer);
					}
					break;
				case West:
					if(s->scene[x][y] == s->scene[x - 1][y] &&
							s->charScene[x - 1][y] == NULL &&
							c->pos[dim_x] > 1){
						c->pos[dim_x] -= 1;
						s->charScene[x][y] = NULL;
						s->charScene[x - 1][y] = c;
						c->cost += costOf(x - 1, y, Wanderer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Wanderer);
					}
					break;
			}
			return 1;
		case Sentry:
			c->cost = INT_MAX;
			return 1;
		case Explorer:
			switch(c->dir){
				case North:
					if(costOf(x, y - 1, Explorer) != INT_MAX &&
							s->charScene[x][y - 1] == NULL &&
							c->pos[dim_y] > 1){
						c->pos[dim_y] -= 1;
						s->charScene[x][y] = NULL;
						s->charScene[x][y - 1] = c;
						c->cost += costOf(x, y - 1, Explorer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Explorer);
					}
					break;
				case South:
					if(costOf(x, y + 1, Explorer) != INT_MAX &&
							s->charScene[x][y + 1] == NULL &&
							c->pos[dim_y] < HEIGHT - 2){
						c->pos[dim_y] += 1;
						s->charScene[x][y] = NULL;
						s->charScene[x][y + 1] = c;
						c->cost += costOf(x, y + 1, Explorer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Explorer);
					}
					break;
				case East:
					if(costOf(x + 1, y, Explorer) != INT_MAX && 
							s->charScene[x + 1][y] == NULL &&
							c->pos[dim_x] < WIDTH - 2){
						c->pos[dim_x] += 1;
						s->charScene[x][y] = NULL;
						s->charScene[x + 1][y] = c;
						c->cost += costOf(x + 1, y, Explorer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Explorer);
					}
					break;
				case West:
					if(costOf(x - 1, y, Explorer) != INT_MAX &&
							s->charScene[x - 1][y] == NULL &&
							c->pos[dim_x] > 1){
						c->pos[dim_x] -= 1;
						s->charScene[x][y] = NULL;
						s->charScene[x - 1][y] = c;
						c->cost += costOf(x - 1, y, Explorer);
					}else{
						c->dir = static_cast<direction_t>(rand() % 4);
						c->cost += costOf(x, y, Explorer);
					}
					break;
			}
			return 1;
		default:
			return 0;
	}
}

//returns 0 to indicate quitting the game
int movePlayer(scene_t *s, character_t *c){
	if(s->scene[player[dim_x]][player[dim_y]] == ':' && rand() % CHNC < 10){
	       	encounter(new pokemon());
		printScene(s);
	}
	while(1){
		//All fall throughs are intentional
		switch(getch()){
			case 'Q':
				return 0;
			case '7':
			case 'y':
				if(s->charScene[player[dim_x] - 1][player[dim_y] - 1] == NULL){
					if(costOf(player[dim_x] - 1, player[dim_y] - 1, PC) != INT_MAX &&
						player[dim_x] > 1 && player[dim_y] > 1){
						c->cost += costOf(player[dim_x] - 1, player[dim_y] - 1, PC);
						c->pos[dim_x] -= 1;
						c->pos[dim_y] -= 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[--player[dim_x]][--player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x] - 1, player[dim_y] - 1, PC) != INT_MAX){
						if(player[dim_y] == 1 && yCord < WORLD_SIZE - 1){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
				      			yCord++;
				       			switchScene(--player[dim_x], HEIGHT - 2);
							return 1;
						}else if(player[dim_x] == 1 && xCord > 0){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
							xCord--;
					       		switchScene(WIDTH - 2, --player[dim_y]);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x] - 1][player[dim_y] - 1]);
				}
				break;
			case '8':
			case 'k':
			case 'w':
				if(s->charScene[player[dim_x]    ][player[dim_y] - 1] == NULL){
					if(costOf(player[dim_x]    , player[dim_y] - 1, PC) != INT_MAX &&
					                     player[dim_y] > 1){
						c->cost += costOf(player[dim_x]    , player[dim_y] - 1, PC);
						c->pos[dim_y] -= 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[  player[dim_x]][--player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x]    , player[dim_y] - 1, PC) != INT_MAX){
						if(player[dim_y] == 1 && yCord < WORLD_SIZE - 1){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
				      			yCord++;
				       			switchScene(player[dim_x], HEIGHT - 2);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x]    ][player[dim_y] - 1]);
				}
				break;
			case '9':
			case 'u':
				if(s->charScene[player[dim_x] + 1][player[dim_y] - 1] == NULL){
					if(costOf(player[dim_x] + 1, player[dim_y] - 1, PC) != INT_MAX &&	
						player[dim_x] < WIDTH - 2 && player[dim_y] > 1){
						c->cost += costOf(player[dim_x] + 1, player[dim_y] - 1, PC);
						c->pos[dim_x] += 1;
						c->pos[dim_y] -= 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[++player[dim_x]][--player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x] + 1, player[dim_y] - 1, PC) != INT_MAX){
						if(player[dim_y] == 1 && yCord < WORLD_SIZE - 1){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
				      			yCord++;
				       			switchScene(++player[dim_x], HEIGHT - 2);
							return 1;
						}else if(player[dim_x] == WIDTH - 2 && xCord < WORLD_SIZE - 1){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
							xCord++;
					       		switchScene(1, --player[dim_y]);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x] + 1][player[dim_y] - 1]);
				}
				break;
			case '6':
			case 'l':
			case 'd':
				if(s->charScene[player[dim_x] + 1][player[dim_y]    ] == NULL){
					if(costOf(player[dim_x] + 1, player[dim_y]    , PC) != INT_MAX &&
						player[dim_x] < WIDTH - 2                     ){
						c->cost += costOf(player[dim_x] + 1, player[dim_y]    , PC);
						c->pos[dim_x] += 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[++player[dim_x]][  player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x] + 1, player[dim_y]    , PC) != INT_MAX){
						if(player[dim_x] == WIDTH - 2 && xCord < WORLD_SIZE - 1){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
							xCord++;
					       		switchScene(1, player[dim_y]);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x] + 1][player[dim_y]    ]);
				}
				break;
			case '3':
			case 'n':
				if(s->charScene[player[dim_x] + 1][player[dim_y] + 1] == NULL){
					if(costOf(player[dim_x] + 1, player[dim_y] + 1, PC) != INT_MAX &&
						player[dim_x] < WIDTH - 2 && player[dim_y] < HEIGHT - 2){
						c->cost += costOf(player[dim_x] + 1, player[dim_y] + 1, PC);
						c->pos[dim_x] += 1;
						c->pos[dim_y] += 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[++player[dim_x]][++player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x] + 1, player[dim_y] + 1, PC) != INT_MAX){
						if(player[dim_y] == HEIGHT - 2 && yCord > 0){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
				      			yCord--;
				       			switchScene(++player[dim_x], 1);
							return 1;
						}else if(player[dim_x] == WIDTH - 2 && xCord < WORLD_SIZE - 1){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
							xCord++;
					       		switchScene(1, ++player[dim_y]);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x] + 1][player[dim_y] + 1]);
				}
				break;
			case '2':
			case 'j':
			case 's':
				if(s->charScene[player[dim_x]    ][player[dim_y] + 1] == NULL){
					if(costOf(player[dim_x]    , player[dim_y] + 1, PC) != INT_MAX &&
					        	             player[dim_y] < HEIGHT - 2){
						c->cost += costOf(player[dim_x]    , player[dim_y] + 1, PC);
						c->pos[dim_y] += 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[  player[dim_x]][++player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x]    , player[dim_y] + 1, PC) != INT_MAX){
						if(player[dim_y] == HEIGHT - 2 && yCord > 0){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
				      			yCord--;
				       			switchScene(player[dim_x], 1);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x]    ][player[dim_y] + 1]);
				}
				break;
			case '1':
			case 'b':
				if(s->charScene[player[dim_x] - 1][player[dim_y] + 1] == NULL){
					if(costOf(player[dim_x] - 1, player[dim_y] + 1, PC) != INT_MAX &&
						player[dim_x] > 1 && player[dim_y] < HEIGHT - 2){
						c->cost += costOf(player[dim_x] - 1, player[dim_y] + 1, PC);
						c->pos[dim_x] -= 1;
						c->pos[dim_y] += 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[--player[dim_x]][++player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x] - 1, player[dim_y] + 1, PC) != INT_MAX){
						if(player[dim_y] == HEIGHT - 2 && yCord > 0){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
				      			yCord--;
				       			switchScene(--player[dim_x], 1);
							return 1;
						}else if(player[dim_x] == 1 && xCord > 0){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
							xCord--;
					       		switchScene(WIDTH - 2, ++player[dim_y]);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x] - 1][player[dim_y] + 1]);
				}
				break;
			case '4':
			case 'h':
			case 'a':
				if(s->charScene[player[dim_x] - 1][player[dim_y]    ] == NULL){
					if(costOf(player[dim_x] - 1, player[dim_y]    , PC) != INT_MAX &&
						player[dim_x] > 1                     ){
						c->cost += costOf(player[dim_x] - 1, player[dim_y]    , PC);
						c->pos[dim_x] -= 1;
						s->charScene[player[dim_x]][player[dim_y]] = NULL;
						s->charScene[--player[dim_x]][  player[dim_y]] = c;
						dijkstra(hikerMap, Hiker);
						dijkstra(rivalMap, Rival);
						return 1;
					}else if(costOf(player[dim_x] - 1, player[dim_y]    , PC) != INT_MAX){
						if(player[dim_x] == 1 && xCord > 0){
							c->cost += costOf(player[dim_x], player[dim_y], PC);
							xCord--;
					       		switchScene(WIDTH - 2, player[dim_y]);
							return 1;
						}
					}	
				}else{
					battle(s->charScene[player[dim_x] - 1][player[dim_y]    ]);
				}
				break;
			case 'B':
				return openBag(false, false);
			case '>':
				if(s->scene[player[dim_x]][player[dim_y]] == 'M'){
					return enterMarket();
				}
				if(s->scene[player[dim_x]][player[dim_y]] == 'C'){
					return enterCenter();
				}
				break;
			case 'f':
				return fly();
				break;
			case '5':
			case ' ':
			case '.':
				c->cost += 10;
				return 1;
			case 't':
				return showTrainers(s);
		}
	}
}

pokemon *pickPokemon(){
	WINDOW *p = newwin(19, 66, 2, 7);

	std::vector<pokemon *>::iterator vi = pcPokemon.begin();
	unsigned int index = 0;
	while(1){
		mvwprintw(p, 1, 24, "%s's stats (%d/%ld)", (*(vi + index))->getIdentifier().c_str(), index + 1, pcPokemon.size());
		mvwprintw(p, 2, 17, "(h/l or a/d to scroll left/right pokemon)");
		mvwprintw(p, 3, 23, "(u to select pokemon)");
		mvwprintw(p, 4, 24, "Species: %s", (*(vi + index))->getSpecies().c_str());
		mvwprintw(p, 5, 24, "Type: %s", (*(vi + index))->getType().c_str());
		
		if(!(*(vi + index))->isAlive()){
			mvwprintw(p, 7, 24, "Currently KO'ed!"); 	
		}
		
		mvwprintw(p, 9, 24, "Level: %d", (*(vi + index))->getLevel());
		mvwprintw(p, 10, 24, "HP: %d/%d", (*(vi + index))->getCurrHp(), (*(vi + index))->getTotalHp());
		mvwprintw(p, 11, 24, "Attack: %d/%d", (*(vi + index))->getCurrAttack(), (*(vi + index))->getTotalAttack());
		mvwprintw(p, 12, 24, "Defense: %d/%d", (*(vi + index))->getCurrDefense(), (*(vi + index))->getTotalDefense());
		mvwprintw(p, 13, 24, "Gender: %s", (*(vi + index))->getGender().c_str());
		mvwprintw(p, 14, 24, "Color: %s", (*(vi + index))->getColor().c_str());
	
		wrefresh(p);
		//fall-through is intentional
		switch(getch()){
			case 'a':
			case 'h':
				if(index > 0) index--;
				break;
			case 'd':
			case 'l':
				if(index < (pcPokemon.size() - 1)) index++;
				break;
			case 'u':
			case 'U':
				delwin(p);
				return *(vi + index);
			default:
				break;
		}
		wclear(p);
	}
}

int openBag(bool inEncounter, bool inBattle){
	WINDOW *b = newwin(19, 66, 2, 7);
	attron(COLOR_PAIR(COLOR_DEFAULT));
	int index = 0;
	while(1){
		mvwprintw(b, 1, 30, "Bag");
		mvwprintw(b, 2, 22, "Arrow keys to select item");
		mvwprintw(b, 3, 22, "exit bag(b/<), use item(u)");
		if(index == 0){
			mvwprintw(b, 5, 23, "->Revives: %d", pcBag.revives);
			mvwprintw(b, 6, 25, "Potions: %d", pcBag.potions);
			mvwprintw(b, 7, 25, "Pokeballs: %d", pcBag.pokeballs);
		}else if(index == 1){
			mvwprintw(b, 5, 25, "Revives: %d", pcBag.revives);
			mvwprintw(b, 6, 23, "->Potions: %d", pcBag.potions);
			mvwprintw(b, 7, 25, "Pokeballs: %d", pcBag.pokeballs);
		}
		else if (index == 2){
			mvwprintw(b, 5, 25, "Revives: %d", pcBag.revives);
			mvwprintw(b, 6, 25, "Potions: %d", pcBag.potions);
			mvwprintw(b, 7, 23, "->Pokeballs: %d", pcBag.pokeballs);
		}
		wrefresh(b);
		
		switch(getch()){
			case 'b':
			case 'B':
			case '<':
				delwin(b);
				return 1;
			case 'u':
			case 'U':
				switch(index){
					pokemon *p;
					case 0:
						mvprintw(0, 0, "                                                                                ");
						if(pcBag.revives <= 0){
							mvprintw(0, 0, "You don't have any revives!");
							break;
						}
						if((p = pickPokemon())->revive()){
							pcBag.revives--;
							mvprintw(0, 0, "Revived %s!", p->getIdentifier().c_str());
						       	if(inEncounter || inBattle){
								return 2;
							}	
						}else{
							mvprintw(0, 0, "%s doesn't need revived!", p->getIdentifier().c_str()); 
						}	
						break;	
					case 1:
						mvprintw(0, 0, "                                                                                ");
						if(pcBag.potions <= 0){
							mvprintw(0, 0, "You don't have any potions!");
							break;
						}
						if((p = pickPokemon())->heal()){
							pcBag.potions--;
							mvprintw(0, 0, "Healed %s!", p->getIdentifier().c_str()); 
						       	if(inEncounter || inBattle){
								return 2;
							}	
						}else{
							mvprintw(0, 0, "%s doesn't need Healed!", p->getIdentifier().c_str()); 
						}
						break;	
					case 2:
						if(pcBag.pokeballs <= 0){
							mvprintw(0, 0, "You don't have any pokeballs!");
							break;
						}
						if(inEncounter){
							return 3;
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "Pokeballs can only be used in encounters");
						}
						break;
				}
				break;
			case KEY_DOWN:
			case 's':
			case 'j':
				if(index < 2) index++;
				break;
			case KEY_UP:
			case 'w':
			case 'k':
				if(index > 0) index--;
				break;
			default:
				break;
		}
		wclear(b);
	}
}

int fly(){
	WINDOW *fly = newwin(19, 42, 2, 18);
	int x, y;
	echo();
	wclear(fly);

	while(1){
		mvwprintw(fly, 1, 15, "Fly/Quick Travel");
		mvwprintw(fly, 3, 5, "Input X Coordinate(-200, 200): ");
		wrefresh(fly);

		if(wscanw(fly, "%d", &x) != 1){
			wclear(fly);
			mvwprintw(fly, 2, 15, "Invalid Input");
			continue;
		}

		x += ORIGIN;

		if(x < WORLD_SIZE && x >= 0){
			xCord = x;

			wclear(fly);
			while(1){
				mvwprintw(fly, 1, 15, "Fly/Quick Travel");
				mvwprintw(fly, 3, 5, "Input Y Coordinate(-200, 200): ");

				if(wscanw(fly, "%d", &y) != 1){
					wclear(fly);
					mvwprintw(fly, 2, 15, "Invalid Input");
					continue;
				}

				y += ORIGIN;
				if(y < WORLD_SIZE && y >= 0){
					yCord = y;
					switchScene(-1, -1);
					noecho();
					delwin(fly);
					return 1;
				} else {
					wclear(fly);
					mvwprintw(fly, 2, 15, "Invalid Input");
				}
			}

		}else{
			wclear(fly);
			mvwprintw(fly, 2, 15, "Invalid Input");
		}			

	}
}

int showTrainers(scene_t *s){
	WINDOW *trainers = newwin(19, 42, 2, 18);
	int offset, i, j;
	offset = 0;
	character_t *trainerList[numTrainers];
	for(i = 1; i < WIDTH - 1; i++){
		for(j = 1; j < HEIGHT - 1; j++){
			if(s->charScene[i][j] != NULL && s->charScene[i][j]->type != PC){
				trainerList[offset] = s->charScene[i][j];
				offset++;
				if(offset == numTrainers) break;
			}	
		}
		if(offset == numTrainers) break;
	}
	offset = 0;
	while(1){
		wclear(trainers);
		mvwprintw(trainers, 1, 15, "Trainer List");
		mvwprintw(trainers, 2, 1, "('Esc' to exit menu or 'Q' to quit game)");
		mvwprintw(trainers, 3, 7, "(Arrow keys to scroll list)");
		for(i = offset; i < numTrainers && i < offset + 14; i++){
			if(trainerList[i]->defeated){
				if(trainerList[i]->pos[dim_x] > player[dim_x]){
					if(trainerList[i]->pos[dim_y] > player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d East, %d South (Defeated)",
							       	getType(trainerList[i]->type), trainerList[i]->pos[dim_x] - player[dim_x],
								trainerList[i]->pos[dim_y] - player[dim_y]);
					}else if(trainerList[i]->pos[dim_y] < player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d East, %d North (Defeated)",
							       	getType(trainerList[i]->type), trainerList[i]->pos[dim_x] - player[dim_x],
								-1*(trainerList[i]->pos[dim_y] - player[dim_y]));
					}else{
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d East (Defeated)",
							       	getType(trainerList[i]->type), trainerList[i]->pos[dim_x] - player[dim_x]);
					}
				}
				else if(trainerList[i]->pos[dim_x] < player[dim_x]){
					if(trainerList[i]->pos[dim_y] > player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d West, %d South (Defeated)",
							       	getType(trainerList[i]->type), -1*(trainerList[i]->pos[dim_x] - player[dim_x]),
								trainerList[i]->pos[dim_y] - player[dim_y]);
					}else if(trainerList[i]->pos[dim_y] < player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d West, %d North (Defeated)",
							       	getType(trainerList[i]->type), -1*(trainerList[i]->pos[dim_x] - player[dim_x]),
								-1*(trainerList[i]->pos[dim_y] - player[dim_y]));
					}else{
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d West (Defeated)",
							       	getType(trainerList[i]->type), -1*(trainerList[i]->pos[dim_x] - player[dim_x]));
					}
				}else{
					if(trainerList[i]->pos[dim_y] > player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d South (Defeated)",
							       	getType(trainerList[i]->type),
								trainerList[i]->pos[dim_y] - player[dim_y]);
					}else{
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d North (Defeated)",
							       	getType(trainerList[i]->type),
								-1*(trainerList[i]->pos[dim_y] - player[dim_y]));
					}
				}
			}else{	
				if(trainerList[i]->pos[dim_x] > player[dim_x]){
					if(trainerList[i]->pos[dim_y] > player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d East, %d South",
							       	getType(trainerList[i]->type), trainerList[i]->pos[dim_x] - player[dim_x],
								trainerList[i]->pos[dim_y] - player[dim_y]);
					}else if(trainerList[i]->pos[dim_y] < player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d East, %d North",
							       	getType(trainerList[i]->type), trainerList[i]->pos[dim_x] - player[dim_x],
								-1*(trainerList[i]->pos[dim_y] - player[dim_y]));
					}else{
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d East",
							       	getType(trainerList[i]->type), trainerList[i]->pos[dim_x] - player[dim_x]);
					}
				}
				else if(trainerList[i]->pos[dim_x] < player[dim_x]){
					if(trainerList[i]->pos[dim_y] > player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d West, %d South",
							       	getType(trainerList[i]->type), -1*(trainerList[i]->pos[dim_x] - player[dim_x]),
								trainerList[i]->pos[dim_y] - player[dim_y]);
					}else if(trainerList[i]->pos[dim_y] < player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d West, %d North",
							       	getType(trainerList[i]->type), -1*(trainerList[i]->pos[dim_x] - player[dim_x]),
								-1*(trainerList[i]->pos[dim_y] - player[dim_y]));
					}else{
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d West",
							       	getType(trainerList[i]->type), -1*(trainerList[i]->pos[dim_x] - player[dim_x]));
					}
				}else{
					if(trainerList[i]->pos[dim_y] > player[dim_y]){
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d South",
							       	getType(trainerList[i]->type),
								trainerList[i]->pos[dim_y] - player[dim_y]);
					}else{
						mvwprintw(trainers, 5 + i - offset, 1, "%s, %d North",
							       	getType(trainerList[i]->type),
								-1*(trainerList[i]->pos[dim_y] - player[dim_y]));
					}
				}
			}
		}	
		wrefresh(trainers);
		
		switch(getch()){
			case 'Q':
				delwin(trainers);
				return 0;
			//27 is the escape character
			case 27:
				delwin(trainers);
				return 1;
			case KEY_DOWN:
			case 'j':
			case 's':
				if(numTrainers > 13 && numTrainers - offset > 13){
					offset += 1;
				}
				break;
			case KEY_UP:
			case 'k':
			case 'w':
				if(offset > 0){
					offset -= 1;
				}
				break;
		}
	}
}

int enterMarket(){
	WINDOW *market = newwin(19, 42, 2, 18);
	mvwprintw(market, 1, 15, "Pokemart");
	mvwprintw(market, 2, 3, "('<' to exit menu or 'Q' to quit game)");
	mvwprintw(market, 5, 12, "Bag Refreshed!");
	pcBag.revives = 3;
	pcBag.potions = 5;
	pcBag.pokeballs = 3;
	wrefresh(market);
	while(1){
		switch(getch()){
			case 'Q':
				delwin(market);
				return 0;
			case '<':
				delwin(market);
				return 1;
		}
	}
}

int enterCenter(){
	WINDOW *center = newwin(19, 42, 2, 18);
	mvwprintw(center, 1, 15, "PokeCenter");
	mvwprintw(center, 2, 3, "('<' to exit menu or 'Q' to quit game)");
	mvwprintw(center, 5, 4, "All pokemon restored to full health!");
	wrefresh(center);
	std::vector<pokemon *>::iterator pi;
	for(pi = pcPokemon.begin(); pi != pcPokemon.end(); pi++){
		(*pi)->center();
	}	
	while(1){
		switch(getch()){
			case 'Q':
				delwin(center);
				return 0;
			case '<':
				delwin(center);
				return 1;
		}
	}
}

const char *getType(unsigned int type){
	switch(type){
		case PC: return	"PC";
		case Hiker: return "Hiker";
		case Rival: return "Rival";
		case Pacer: return "Pacer";
		case Wanderer: return "Wanderer";
		case Sentry: return "Sentry";
		case Explorer: return "Explorer";
		default: return "Unknown";
	}
}


unsigned int pickCurrPokemon(){
	WINDOW *p = newwin(19, 66, 2, 7);

	attron(COLOR_PAIR(COLOR_DEFAULT));
	std::vector<pokemon *>::iterator vi = pcPokemon.begin();
	unsigned int index = 0;
	while(1){
		mvwprintw(p, 1, 24, "%s's stats (%d/%ld)", (*(vi + index))->getIdentifier().c_str(), index + 1, pcPokemon.size());
		mvwprintw(p, 2, 17, "(h/l or a/d to scroll left/right pokemon)");
		mvwprintw(p, 3, 23, "(u to select pokemon)");
		mvwprintw(p, 4, 24, "Species: %s", (*(vi + index))->getSpecies().c_str());
		mvwprintw(p, 5, 24, "Type: %s", (*(vi + index))->getType().c_str());
		
		if(!(*(vi + index))->isAlive()){
			mvwprintw(p, 7, 24, "Currently KO'ed!"); 	
		}
		
		mvwprintw(p, 9, 24, "Level: %d", (*(vi + index))->getLevel());
		mvwprintw(p, 10, 24, "HP: %d/%d", (*(vi + index))->getCurrHp(), (*(vi + index))->getTotalHp());
		mvwprintw(p, 11, 24, "Attack: %d/%d", (*(vi + index))->getCurrAttack(), (*(vi + index))->getTotalAttack());
		mvwprintw(p, 12, 24, "Defense: %d/%d", (*(vi + index))->getCurrDefense(), (*(vi + index))->getTotalDefense());
		mvwprintw(p, 13, 24, "Gender: %s", (*(vi + index))->getGender().c_str());
		mvwprintw(p, 14, 24, "Color: %s", (*(vi + index))->getColor().c_str());
	
		wrefresh(p);
		//fall-through is intentional
		switch(getch()){
			case 'a':
			case 'h':
				if(index > 0) index--;
				break;
			case 'd':
			case 'l':
				if(index < (pcPokemon.size() - 1)) index++;
				break;
			case 'u':
			case 'U':
				if((*(vi + index))->isAlive()){
					delwin(p);
					return index;
				} else{
					mvprintw(0, 0, "                                                                                ");
					mvprintw(0, 0, "Cannot use a pokemon that is KO'ed!"); 
				}
				break;
			default:
				break;
		}
		wclear(p);
	}
}

ability *pcPickMove(pokemon *p){
	WINDOW *w = newwin(19, 66, 2, 7);

	attron(COLOR_PAIR(COLOR_DEFAULT));
	std::vector<ability *> pokeAbilities = p->getAbilities();
	std::vector<ability *>::iterator ai = pokeAbilities.begin();
	unsigned int index = 0;
	while(1){
		mvwprintw(w, 1, 24, "%s's moves (%d/%ld)", p->getIdentifier().c_str(), index + 1, pokeAbilities.size());
		mvwprintw(w, 2, 17, "(h/l or a/d to scroll left/right abilities)");
		mvwprintw(w, 3, 23, "(u to select move)");
		
		mvwprintw(w, 5, 24, "Move: %s", (*(ai + index))->getIdentifier().c_str());
		mvwprintw(w, 7, 24, "Type: %s", (*(ai + index))->getType().c_str());
		
		mvwprintw(w, 9, 24, "Power: %d", (*(ai + index))->getPower());
		mvwprintw(w, 11, 24, "Accuracy: %d/100", (*(ai + index))->getAccuracy());
	
		wrefresh(w);
		//fall-through is intentional
		switch(getch()){
			case 'a':
			case 'h':
			case KEY_LEFT:
				if(index > 0) index--;
				break;
			case 'd':
			case 'l':
			case KEY_RIGHT:
				if(index < (pokeAbilities.size() - 1)) index++;
				break;
			case 'u':
			case 'U':
			case ' ':
				delwin(w);
				return *(ai + index);
			default:
				break;
		}
		wclear(w);
	}
}

bool pcDefeated(){
	unsigned int i;
	std::vector<pokemon *>::iterator pi = pcPokemon.begin();
	for(i = 0; (pi + i) != pcPokemon.end(); i++){
		if((*(pi + i))->isAlive()){
			return false;
		}
	}
	return true;	
}

ability *pickBestMove(pokemon *attacker, pokemon *defender){
	std::vector<ability *>::iterator ai;
	std::vector<ability *> pokeAbilities = attacker->getAbilities();
	
	unsigned int bestindex, index = 0;
	int amount, mostDamage = 0;
	for(ai = pokeAbilities.begin(); (ai + index) != pokeAbilities.end(); index++){
		if((amount = useAbility(*(ai + index), attacker, defender)) > mostDamage){
			mostDamage = amount;
			bestindex = index;
		}
	}
	return *(ai + bestindex);
}

void battle(character_t *c){
	if(c->defeated){
		return;
	}
	attron(COLOR_PAIR(COLOR_DEFAULT));
	
	mvprintw(0, 0, "                                                                                ");
	mvprintw(0, 0, "Battling a %s", getType(c->type));
	
	WINDOW *fight = newwin(20, 66, 2, 7);
	std::vector<pokemon *>::iterator vi = c->pets.begin();
	std::vector<pokemon *>::iterator pi = pcPokemon.begin();

	unsigned int trPoke = 0;
	int index = 0;
	bool trainerAttacking = true;
	while(1){
		if(!(*(vi + trPoke))->isAlive()){ 
			trainerAttacking = false;
			mvprintw(0, 0, "                                                                                ");
			mvprintw(0, 0, "%s is KO'ed!", (*(vi + trPoke))->getIdentifier().c_str());
			if(trPoke < (c->pets.size() - 1)){
			       	trPoke++;
				mvprintw(0, 0, "                                                                                ");
				mvprintw(0, 0, "Trainer switches in %s!", (*(vi + trPoke))->getIdentifier().c_str());
			}
			else{
				mvprintw(0, 0, "                                                                                ");
				mvprintw(0, 0, "%s defeated!", getType(c->type));
				c->defeated = 1;
				delwin(fight);
				return;
			}	
		}else{
			trainerAttacking = true;
		}
		if(pcDefeated()){
			mvprintw(0, 0, "                                                                                ");
			mvprintw(0, 0, "You have been defeated!!!");
			delwin(fight);
			return;
		}
		 mvwprintw(fight, 1, 5, "You: %s", (*(pi + currPoke))->getIdentifier().c_str());
		 mvwprintw(fight, 2, 5, "Species: %s", (*(pi + currPoke))->getSpecies().c_str());
		 mvwprintw(fight, 3, 5, "Type: %s", (*(pi + currPoke))->getType().c_str());
		 mvwprintw(fight, 4, 5, "Level: %d", (*(pi + currPoke))->getLevel());
		 mvwprintw(fight, 5, 5, "HP: %d/%d", (*(pi + currPoke))->getCurrHp(), (*(pi + currPoke))->getTotalHp());
		 mvwprintw(fight, 6, 5, "Attack: %d/%d", (*(pi + currPoke))->getCurrAttack(), (*(pi + currPoke))->getTotalAttack());
		 mvwprintw(fight, 7, 5, "Defense: %d/%d", (*(pi + currPoke))->getCurrDefense(), (*(pi + currPoke))->getTotalDefense());
	
		 mvwprintw(fight, 1, 38, "VS (%d/%ld): %s", trPoke + 1, c->pets.size(), (*(vi + trPoke))->getIdentifier().c_str());
		 mvwprintw(fight, 2, 38, "Species: %s", (*(vi + trPoke))->getSpecies().c_str());
		 mvwprintw(fight, 3, 38, "Type: %s", (*(vi + trPoke))->getType().c_str());
		 mvwprintw(fight, 4, 38, "Level: %d", (*(vi + trPoke))->getLevel());
		 mvwprintw(fight, 5, 38, "HP: %d/%d", (*(vi + trPoke))->getCurrHp(), (*(vi + trPoke))->getTotalHp());
		 mvwprintw(fight, 6, 38, "Attack: %d/%d", (*(vi + trPoke))->getCurrAttack(), (*(vi + trPoke))->getTotalAttack());
		 mvwprintw(fight, 7, 38, "Defense: %d/%d", (*(vi + trPoke))->getCurrDefense(), (*(vi + trPoke))->getTotalDefense());

		if(index == 0){
			mvwprintw(fight, 11, 9, "->Fight        Bag         Flee        Pokemon");
		}else if(index == 1){
			mvwprintw(fight, 11, 9, "  Fight      ->Bag         Flee        Pokemon");
		}else if(index == 2){
			mvwprintw(fight, 11, 9, "  Fight        Bag       ->Flee        Pokemon");
		}else{
			mvwprintw(fight, 11, 9, "  Fight        Bag         Flee      ->Pokemon");
		}
		wrefresh(fight);
		
		switch(getch()){
			case 'u':
			case 'U':
			case ' ':
				if(!(*(pi + currPoke))->isAlive() && index != 3){
					mvprintw(0, 0, "                                                                                ");
					mvprintw(0, 0, "Cannot perform action while current pokemon is KO'ed! (switch pokemon)");
					break;
				}
				if(index == 0){
					//Select ability to attack with
					ability *bestMove = pickBestMove(*(vi + trPoke), *(pi + currPoke));
					ability *pcMove = pcPickMove(*(pi + currPoke));	
						
					int amount;
					if(pcMove->getPriority() < bestMove->getPriority() || ((pcMove->getPriority() == bestMove->getPriority()) && ((*(vi + trPoke))->getCurrSpeed() > (*(pi + currPoke))->getCurrSpeed()))){		
						//trainer poke goes first
						if(rand() % CHNC < bestMove->getAccuracy()){
							amount = useAbility(bestMove, *(vi + trPoke), *(pi + currPoke));
							(*(pi + currPoke))->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(vi + trPoke))->getIdentifier().c_str(), bestMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", (*(vi + trPoke))->getIdentifier().c_str());
						}
						
						if(!(*(pi + currPoke))->isAlive()){
							break;
						}	

						if(rand() % CHNC < pcMove->getAccuracy()){
							amount = useAbility(pcMove, *(pi + currPoke), (*(vi + trPoke)));
							(*(vi + trPoke))->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(pi + currPoke))->getIdentifier().c_str(), pcMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", (*(pi + currPoke))->getIdentifier().c_str());
						}
					} else {
						//pc goes first
						if(rand() % CHNC < pcMove->getAccuracy()){
							amount = useAbility(pcMove, *(pi + currPoke), (*(vi + trPoke)));
							(*(vi + trPoke))->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(pi + currPoke))->getIdentifier().c_str(), pcMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", (*(pi + currPoke))->getIdentifier().c_str());
						}
						
						if(!(*(vi + trPoke))->isAlive()){
							break;
						}

						if(rand() % CHNC < bestMove->getAccuracy()){
							amount = useAbility(bestMove, (*(vi + trPoke)), *(pi + currPoke));
							(*(pi + currPoke))->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(vi + trPoke))->getIdentifier().c_str(), bestMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", (*(vi + trPoke))->getIdentifier().c_str());
						}

					}
					break;
				}else if(index == 1){
					//select item from bag
					if(openBag(false, true) == 1) break;
				}else if(index == 2){
					//attempt to flee
					mvprintw(0, 0, "                                                                                ");
					mvprintw(0, 0, "Cannot flee from battles!!");
					break;
				}else{
					currPoke = pickCurrPokemon();
				}
				if(trainerAttacking){
					//pokemon turn to attack
					ability *bestMove = pickBestMove((*(vi + trPoke)), *(pi + currPoke));
					if(rand() % CHNC < bestMove->getAccuracy()){
						int amount = useAbility(bestMove, (*(vi + trPoke)), *(pi + currPoke));
						(*(pi + currPoke))->receiveDamage(amount);

						mvprintw(0, 0, "                                                                                ");
						mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(vi + trPoke))->getIdentifier().c_str(), bestMove->getIdentifier().c_str(), amount);
					}else{
						mvprintw(0, 0, "                                                                                ");
						mvprintw(0, 0, "%s misses!!", (*(vi + trPoke))->getIdentifier().c_str());
					}
				}
				break;
			case KEY_RIGHT:
			case 'd':
			case 'l':
				if(index < 3) index++;
				break;
			case KEY_LEFT:
			case 'a':
			case 'h':
				if(index > 0) index--;
				break;
			default:
				break;
		}
		wclear(fight);
	}
}


void encounter(pokemon *p){
	
	WINDOW *fight = newwin(19, 66, 2, 7);
	attron(COLOR_PAIR(COLOR_DEFAULT));
	
	mvprintw(0, 0, "                                                                                ");
	mvprintw(0, 0, "Encountered a wild %s!", p->getIdentifier().c_str() );

	std::vector<pokemon *>::iterator pi = pcPokemon.begin();

	int index = 0;
	while(1){
		if(!p->isAlive()){ 
			mvprintw(0, 0, "                                                                                ");
			mvprintw(0, 0, "%s is KO'ed!", p->getIdentifier().c_str());
			delwin(fight);
			delete p;
			return;
		}
		if(pcDefeated()){
			mvprintw(0, 0, "                                                                                ");
			mvprintw(0, 0, "You have been defeated!!!");
			delwin(fight);
			delete p;
			return;
		}
		 mvwprintw(fight, 1, 5, "You: %s", (*(pi + currPoke))->getIdentifier().c_str());
		 mvwprintw(fight, 2, 5, "Species: %s", (*(pi + currPoke))->getSpecies().c_str());
		 mvwprintw(fight, 3, 5, "Type: %s", (*(pi + currPoke))->getType().c_str());
		 mvwprintw(fight, 4, 5, "Level: %d", (*(pi + currPoke))->getLevel());
		 mvwprintw(fight, 5, 5, "HP: %d/%d", (*(pi + currPoke))->getCurrHp(), (*(pi + currPoke))->getTotalHp());
		 mvwprintw(fight, 6, 5, "Attack: %d/%d", (*(pi + currPoke))->getCurrAttack(), (*(pi + currPoke))->getTotalAttack());
		 mvwprintw(fight, 7, 5, "Defense: %d/%d", (*(pi + currPoke))->getCurrDefense(), (*(pi + currPoke))->getTotalDefense());
	
		 mvwprintw(fight, 1, 38, "VS: %s", p->getIdentifier().c_str());
		 mvwprintw(fight, 2, 38, "Species: %s", p->getSpecies().c_str());
		 mvwprintw(fight, 3, 38, "Type: %s", p->getType().c_str());
		 mvwprintw(fight, 4, 38, "Level: %d", p->getLevel());
		 mvwprintw(fight, 5, 38, "HP: %d/%d", p->getCurrHp(), p->getTotalHp());
		 mvwprintw(fight, 6, 38, "Attack: %d/%d", p->getCurrAttack(), p->getTotalAttack());
		 mvwprintw(fight, 7, 38, "Defense: %d/%d", p->getCurrDefense(), p->getTotalDefense());

		if(index == 0){
			mvwprintw(fight, 11, 9, "->Fight        Bag         Flee        Pokemon");
		}else if(index == 1){
			mvwprintw(fight, 11, 9, "  Fight      ->Bag         Flee        Pokemon");
		}else if(index == 2){
			mvwprintw(fight, 11, 9, "  Fight        Bag       ->Flee        Pokemon");
		}else{
			mvwprintw(fight, 11, 9, "  Fight        Bag         Flee      ->Pokemon");
		}
		wrefresh(fight);
		
		switch(getch()){
			case 'u':
			case 'U':
			case ' ':
				if(!(*(pi + currPoke))->isAlive() && index != 3){
					mvprintw(0, 0, "                                                                                ");
					mvprintw(0, 0, "Cannot perform action while current pokemon is KO'ed! (switch pokemon)");
					break;
				}
				if(index == 0){
					//Select ability to attack with
					ability *bestMove = pickBestMove(p, *(pi + currPoke));
					ability *pcMove = pcPickMove(*(pi + currPoke));	
						
					int amount;
					if(pcMove->getPriority() < bestMove->getPriority() || ((pcMove->getPriority() == bestMove->getPriority()) && (p->getCurrSpeed() > (*(pi + currPoke))->getCurrSpeed()))){		
						//wild poke goes first
						if(rand() % CHNC < bestMove->getAccuracy()){
							amount = useAbility(bestMove, p, *(pi + currPoke));
							(*(pi + currPoke))->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", p->getIdentifier().c_str(), bestMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", p->getIdentifier().c_str());
						}
						
						if(!(*(pi + currPoke))->isAlive()){
							break;
						}	

						if(rand() % CHNC < pcMove->getAccuracy()){
							amount = useAbility(pcMove, *(pi + currPoke), p);
							p->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(pi + currPoke))->getIdentifier().c_str(), pcMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", (*(pi + currPoke))->getIdentifier().c_str());
						}
					} else {
						//pc goes first
						if(rand() % CHNC < pcMove->getAccuracy()){
							amount = useAbility(pcMove, *(pi + currPoke), p);
							p->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", (*(pi + currPoke))->getIdentifier().c_str(), pcMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", (*(pi + currPoke))->getIdentifier().c_str());
						}
						
						if(!p->isAlive()){
							break;
						}

						if(rand() % CHNC < bestMove->getAccuracy()){
							amount = useAbility(bestMove, p, *(pi + currPoke));
							(*(pi + currPoke))->receiveDamage(amount);

							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s uses %s dealing %d damage!", p->getIdentifier().c_str(), bestMove->getIdentifier().c_str(), amount);
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s misses!!", p->getIdentifier().c_str());
						}

					}
					break;
				}else if(index == 1){
					//select item from bag
					int value = openBag(true, false);
					
					//opened and closed bag not using item
					if(value == 1) break;
					
					//using pokeball
					if(value == 3){
						if(pcPokemon.size() >= 6){
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "Already have max pokemon, cannot catch more!");
							break;
						}
						pcBag.pokeballs--;
						if(rand() % CHNC < CHNC_CAPTURE){
							p->center();
							pcPokemon.push_back(p);
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "Captured %s!", p->getIdentifier().c_str());
							delwin(fight);
							return;
						}else{
							mvprintw(0, 0, "                                                                                ");
							mvprintw(0, 0, "%s evaded capture!", p->getIdentifier().c_str());
						}	
					}		
				}else if(index == 2){
					//attempt to flee
					if(rand() % CHNC < CHNC_FLEE){
						mvprintw(0, 0, "                                                                                ");
						mvprintw(0, 0, "Fled from %s!", p->getIdentifier().c_str());
						delete p;
						delwin(fight);
						return;
					} else {
						mvprintw(0, 0, "                                                                                ");
						mvprintw(0, 0, "%s stopped you from fleeing!!", p->getIdentifier().c_str());
					}
				}else{
					currPoke = pickCurrPokemon();
				}
				{
					//wild pokemon turn to attack, {} to denote lifetime of variables
					ability *bestMove = pickBestMove(p, *(pi + currPoke));
					if(rand() % CHNC < bestMove->getAccuracy()){
						int amount = useAbility(bestMove, p, *(pi + currPoke));
						(*(pi + currPoke))->receiveDamage(amount);

						mvprintw(0, 0, "                                                                                ");
						mvprintw(0, 0, "%s uses %s dealing %d damage!", p->getIdentifier().c_str(), bestMove->getIdentifier().c_str(), amount);
					}else{
						mvprintw(0, 0, "                                                                                ");
						mvprintw(0, 0, "%s misses!!", p->getIdentifier().c_str());
					}
				}
				break;
			case KEY_RIGHT:
			case 'd':
			case 'l':
				if(index < 3) index++;
				break;
			case KEY_LEFT:
			case 'a':
			case 'h':
				if(index > 0) index--;
				break;
			default:
				break;
		}
		wclear(fight);
	}
}

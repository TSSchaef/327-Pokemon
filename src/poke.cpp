#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <unistd.h>
#include <ncurses.h>
#include <vector>

#include "poke.h"

int seed, xCord, yCord;
int numTrainers = 10;
scene_t *world[WORLD_SIZE][WORLD_SIZE];
std::vector<pokemon *> pcPokemon;
unsigned int currPoke = 0;
bag pcBag;


std::vector<poke_class *> pokemon_datas, moves, pokemon_moves, species, experiences, type_names, pokemon_stats, stats, pokemon_types;

int main(int argc, char *argv[]){
	std::string *filepath = findDB();
	if(filepath == NULL){
		std::cout << "Pokedex DB not found" << std::endl;
	       	return 0;
	}
	
	
	pokemon_datas = scanPokemon(*filepath);
	moves = scanMoves(*filepath);
	pokemon_moves = scanPokeMoves(*filepath);
	species = scanPokeSpecies(*filepath);
	experiences = scanExperience(*filepath);
	type_names = scanTypeNames(*filepath);
	pokemon_stats = scanPokeStats(*filepath);
	stats = scanStats(*filepath);
	pokemon_types = scanPokeTypes(*filepath);
	
	delete filepath;

	int prevX, prevY;
	character_t *c;
	
	if(argc >= 3 && strcmp(argv[1], "--numtrainers") == 0){
		numTrainers = atoi(argv[2]);
	}
	if(argc == 4){
		seed = atoi(argv[3]);
	}
	
	srand(getSeed());
	
	startWorld();

	while((c = (character_t*) heap_remove_min(&(world[xCord][yCord]->eventDriver)))){
		c->hn = NULL;
		
		prevX = xCord;
		prevY = yCord;

		if(!moveCharacter(world[xCord][yCord], c) || pcDefeated()){
			heap_insert(&(world[prevX][prevY]->eventDriver), c);
			break;
		}
		
		heap_insert(&(world[prevX][prevY]->eventDriver), c);
	}
	bool defeated = pcDefeated();

	endWorld();	

	endwin();
	if(defeated) printf("GAME OVER: You were defeated!!!\n");	
	return 0;
}


static int32_t char_cmp(const void *key, const void *with) {
	return ((character_t *) key)->cost - ((character_t *) with)->cost;
}

void initScreen(){
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(COLOR_ROCK, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_PATH, COLOR_BLACK, COLOR_YELLOW);
	init_pair(COLOR_TREE,  COLOR_BLACK, COLOR_GREEN);
	init_pair(COLOR_WATER, COLOR_CYAN, COLOR_BLUE);
	init_pair(COLOR_SGRASS, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_TGRASS, COLOR_YELLOW, COLOR_BLACK);
	init_pair(COLOR_BUILDING, COLOR_BLACK, COLOR_RED);
	init_pair(COLOR_TRAINER, COLOR_RED, COLOR_WHITE);
	init_pair(COLOR_PC, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
}

//output scene to terminal
void printScene(scene_t *s){
	int i, j;
	for(i = 0; i < HEIGHT; i++){
		for(j = 0; j < WIDTH; j++){
			if(s->charScene[j][i] != NULL){
				char c;
				attron(COLOR_PAIR(COLOR_TRAINER));
				switch(s->charScene[j][i]->type){
					case PC:
						c = '@';
						attron(COLOR_PAIR(COLOR_PC));
						break;
					case Hiker:
						c = 'h';
						break;
					case Rival:
						c = 'r';
						break;
					case Pacer:
						c = 'p';
						break;
					case Wanderer:
						c = 'w';
						break;
					case Sentry:
						c = 's';
						break;
					case Explorer:
						c = 'e';
						break;
				}
				mvaddch(i + 1, j, c);	
			}else {	
			 	switch(s->scene[j][i]){
					case '%':
						attron(COLOR_PAIR(COLOR_ROCK));
						break;
					case '^':
						attron(COLOR_PAIR(COLOR_TREE));
						break;
					case '~':
						attron(COLOR_PAIR(COLOR_WATER));
						break;
					case ':':
						attron(COLOR_PAIR(COLOR_TGRASS));
						break;
					case '.':
						attron(COLOR_PAIR(COLOR_SGRASS));
						break;
					case 'M':
					case 'C':
						attron(COLOR_PAIR(COLOR_BUILDING));
						break;
					case '#':
						attron(COLOR_PAIR(COLOR_PATH));
						break;
				}	
				//i + 1 to leave an empty line at the top
				mvaddch(i + 1, j, s->scene[j][i]);
			}
		}
	}
	refresh();	
}

//fill border of a scene with rocks
void fillBorder(scene_t *s){
	int i, j;
	for(i = 0, j = 0; i < HEIGHT; i++){
		s->scene[j][i] = '%';
	}
	for(j = 0, i = 0; j < WIDTH; j++){
		s->scene[j][i] = '%';
	}
	for(i = HEIGHT - 1, j = WIDTH - 1; i >= 0; i--){
		s->scene[j][i] = '%';
	}
	for(j = WIDTH - 1, i = HEIGHT - 1; j >= 0; j--){
		s->scene[j][i] = '%';
	}
}

//generate single random seed
int getSeed(){
	if(seed == 0){
		srand(time(NULL));
		seed = rand();
	}
	return seed;
}

//clear the given scene
void clearScene(scene_t *s){
	int i, j;
	for(i = 0; i < HEIGHT; i++){
		for(j = 0; j < WIDTH; j++){
			s->scene[j][i] = '\0';
			s->charScene[j][i] = NULL;
		}
	}

}

//generate psuedo-random terrain
void generateTerrain(scene_t *s){
	clearScene(s);
	int i, x, y, randChnc;
	int fullScene = 0;
	
	//set seeds
	for(i = 0; i < 12; i++){
		do{
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
		}while(s->scene[x][y] != '\0');

		if(i < 2){
			s->scene[x][y] = '%';
		}else if(i < 4){
			s->scene[x][y] = '~';
		}else if(i < 6){
			s->scene[x][y] = '^';
		}else if(i < 9){
			s->scene[x][y] = ':';
		}else{
			s->scene[x][y] = '.';
		}
	}
	
	//grow seeds
	while(!fullScene){
		fullScene = 1;
		for(x = 0; x < WIDTH; x++){
			for(y = 0; y < HEIGHT; y++){
				if(s->scene[x][y] != '\0'){
					if(s->scene[x][y] == '%') randChnc = CHNC_ROCK_SPREAD;
					if(s->scene[x][y] == '~') randChnc = CHNC_WATER_SPREAD;
					if(s->scene[x][y] == '^') randChnc = CHNC_TREE_SPREAD;
					if(s->scene[x][y] == ':') randChnc = CHNC_WEEDS_SPREAD;
					if(s->scene[x][y] == '.') randChnc = CHNC_GRASS_SPREAD;

					if(x < WIDTH - 1 && s->scene[x + 1][y] == '\0' && rand() % CHNC < randChnc){
						s->scene[x + 1][y] = s->scene[x][y];
					}
					if(y < HEIGHT - 1 && s->scene[x][y + 1] == '\0' && rand() % CHNC < randChnc){
						s->scene[x][y + 1] = s->scene[x][y];
					}
					if(x > 0 && s->scene[x - 1][y] == '\0' && rand() % CHNC < randChnc){
						s->scene[x - 1][y] = s->scene[x][y];
					}
					if(y > 0 && s->scene[x][y - 1] == '\0' && rand() % CHNC < randChnc){
						s->scene[x][y - 1] = s->scene[x][y];
					}
				}else{
					fullScene = 0;
				}
			}
		}
	}
}

//generate paths across the terrain, one E-W and one N-S
//if map is at the edge of the world will generate paths
//only between existing gates
void generatePaths(scene_t *s){
	int dif, x, y, dir, cntrX, cntrY;
	
	//(# - 2) + 1 to avoid having paths end in the corners of the map
	s->w = (s->w == -1)? rand() % (HEIGHT - 2) + 1: s->w;
	s->s = (s->s == -1)? rand() % (WIDTH - 2) + 1: s->s;
	s->n = (s->n == -1)? rand() % (WIDTH - 2) + 1: s->n;
	s->e = (s->e == -1)? rand() % (HEIGHT - 2) + 1: s->e;

	cntrY = (HEIGHT / 2) + ((rand() % 8) - 4);
	
	if(s->x > 0 && s->x < WORLD_SIZE - 1){
		cntrX = (WIDTH / 2) + ((rand() % 20) - 10);
	}else if(s->x > 0){
		cntrX = (s->n > s->s)? s->n : s->s;
		cntrY = s->w;
	}else{
		cntrX = (s->n < s->s)? s->n : s->s;
		cntrY = s->e;
	}	
	
	if(s->y == 0) cntrX = s->n;
	if(s->y == WORLD_SIZE - 1) cntrX = s->s;

	s->scene[cntrX][cntrY] = '#';
	

	//building path west to center
	if(s->x > 0){
		y = s->w;
		s->scene[0][s->w] = '#';
		for(x = 1; x <  cntrX; x++){
			dif = y - cntrY;
			if(dif == 0){
				dir = rand() % (cntrX - x);
			}else if(dif > (cntrX - x) || -1 * dif > (cntrX - x)){
				dir = 1;		
			}else if(dif < 0){	
				dir = rand() % ((cntrX - x) / (- 1 * dif));
			}else{
				dir = rand() % ((cntrX - x) / dif);
			}

			s->scene[x][y] = '#';
			if(dir < 2 && dif != 0){
				y += dif > 0? -1: 1;
				s->scene[x][y] = '#';
			}
			if(dir > 20 && y < HEIGHT - 2 && y > 2){
				y += dif > 0? 1: -1;
				s->scene[x][y] = '#';
			}	
		}
	}
	       		
	//path east to center
	if(s->x < WORLD_SIZE - 1){
		y = s->e;
		s->scene[WIDTH - 1][s->e] = '#';
		for(x = WIDTH - 2; x > cntrX; x--){
			dif = y - cntrY;
			if(dif == 0){
				dir = rand() % (x - cntrX);
			}else if(dif > (x - cntrX) || -1 * dif > (x - cntrX)){
				dir = 1;	
			}else if(dif < 0){
				dir = rand() % ((x - cntrX) / (-1 * dif));
			}else{
				dir = rand() % ((x - cntrX) / dif);
			}

			s->scene[x][y] = '#';
			if(dir < 2 && dif != 0){
				y += dif > 0? -1: 1;
				s->scene[x][y] = '#';
			}
			if(dir > 20 && y < HEIGHT - 2 && y > 2){
				y += dif > 0? 1: -1;
				s->scene[x][y] = '#';
			}	
		}
	}

	//north to center
	if(s->y < WORLD_SIZE - 1){
		int i;
		for(i = 0; i < HEIGHT; i++){
			if(s->scene[s->n][i] == '#'){
				cntrY = i;
				break;
			}
		}
		cntrX = s->n;
		x = s->n;
		s->scene[s->n][0] = '#';
		for(y = 1; y <  cntrY; y++){
			dif = x - cntrX;
			if(dif == 0){
				dir = rand() % (cntrY - y);
			}else if(dif < 0){	
				dir = rand() % ((cntrY - y) / (- 1 * dif));
			}else{
				dir = rand() % ((cntrY - y) / dif);
			}

			s->scene[x][y] = '#';
			if(dir < 1 && dif != 0){
				x += dif > 0? -1: 1;
				s->scene[x][y] = '#';
			}
			if(dir > 8 && x < WIDTH - 2 && x > 2){
				x += dif > 0? 1: -1;
				s->scene[x][y] = '#';
			}	
		}
	}

	//south to center
	if(s->y > 0){	
		int i;
		for(i = HEIGHT - 1; i > 0; i--){
			if(s->scene[s->s][i] == '#'){
				cntrY = i;
				break;
			}
		}
		cntrX = s->s;
		x = s->s;
		s->scene[s->s][HEIGHT - 1] = '#';
		for(y = HEIGHT - 2; y > cntrY; y--){
			dif = x - cntrX;
			if(dif == 0){
				dir = rand() % (cntrY - y);
			}else if(dif < 0){	
				dir = rand() % ((cntrY - y) / (-1 * dif));
			}else{
				dir = rand() % ((cntrY - y) / dif);
			}

			s->scene[x][y] = '#';
			if(dir < 1 && dif != 0){
				x += dif > 0? -1: 1;
				s->scene[x][y] = '#';
			}
			if(dir > 8 && x < WIDTH - 2 && x > 2){
				x += dif > 0? 1: -1;
				s->scene[x][y] = '#';
			}	
		}
	}

}


//places buildings along path
void placeBuildings(scene_t *s){
	int numPlaced = 2;
	char type;
	int x, y, d;
	
	//guaranteed buildings at ORIGIN
	if(s->x == ORIGIN && s->y == ORIGIN){
		numPlaced = 0;
		type = 'C';
	}else if((d = abs(s->x - ORIGIN) + abs(s->y - ORIGIN)) < ORIGIN){
		d *= CHNC_BLDNG_EDGE - CHNC_BLDNG_CENTER;
		d /= ORIGIN;
		d += CHNC_BLDNG_CENTER;
		//linear chance to proc 50% near ORIGIN to 5% at 200 away (Manhattan distance)	
		if(rand() % CHNC < d){
			numPlaced--;
			type = 'M';
		}
		if(rand() % CHNC < d){
			numPlaced--;
			type = 'C';
		}
	}else{
		//5% chance to proc
		if(rand() % CHNC < CHNC_BLDNG_EDGE){
			numPlaced--;
			type = 'M';
		}
		if(rand() % CHNC < CHNC_BLDNG_EDGE){
			numPlaced--;
			type = 'C';
		}
	}
	
	while(numPlaced != 2){
		//keep buildings towards center of map
		x = (rand() % (WIDTH - 10)) + 5;
		y = (rand() % (HEIGHT - 2)) + 1;

		if(s->scene[x][y] == '#'){
			if(y > 2 && s->scene[x][y - 1] != '#' && s->scene[x + 1][y - 1] != '#'
					&& s->scene[x][y - 2] != '#' && s->scene[x + 1][y - 2] != '#'
					&& s->scene[x][y - 1] != 'C' && s->scene[x + 1][y - 1] != 'C'
					&& s->scene[x][y - 2] != 'C' && s->scene[x + 1][y - 2] != 'C'){
				
				s->scene[x][y - 1] = type;
				s->scene[x + 1][y - 1] = type;
				s->scene[x][y - 2] = type;
			       	s->scene[x + 1][y - 2] = type;
				
				type = 'M';
				numPlaced++;
			}else if(y < HEIGHT - 3 && s->scene[x][y + 1] != '#' && s->scene[x + 1][y + 1] != '#'
					&& s->scene[x][y + 2] != '#' && s->scene[x + 1][y + 2] != '#'
					&& s->scene[x][y + 1] != 'C' && s->scene[x + 1][y + 1] != 'C'
					&& s->scene[x][y + 2] != 'C' && s->scene[x + 1][y + 2] != 'C'){
				
				s->scene[x][y + 1] = type;
				s->scene[x + 1][y + 1] = type;
				s->scene[x][y + 2] = type;
			       	s->scene[x + 1][y + 2] = type;
				
				type = 'M';
				numPlaced++;
			}
		}
	}
}

//generates a scene at the given pointer
void buildScene(scene_t *s, int x, int y){
	generateTerrain(s);
	fillBorder(s);
	generatePaths(s);
	placeBuildings(s);
	heap_init(&(s->eventDriver), char_cmp, NULL);					
	placePlayer(world[xCord][yCord], x, y);
	dijkstra(hikerMap, Hiker);
	dijkstra(rivalMap, Rival);
	placeNPCs(numTrainers, world[xCord][yCord]);
}

void pickPoke(){
	WINDOW *pick = newwin(19, 66, 2, 7);
	mvwprintw(pick, 2, 20, "Some lore or something...");
	mvwprintw(pick, 4, 20, "Select a starter pokemon:");
	mvwprintw(pick, 6, 11, "Bulbasaur        Charmander        Squirtle");
	mvwprintw(pick, 7, 11, "   (b)               (c)              (s)");
	while(1){	
		wrefresh(pick);
		switch(getch()){
			case 'b':
				pcPokemon.push_back(new pokemon(1));
				mvprintw(0, 0, "Selected Bulbasaur");
				delwin(pick);
				return;
			case 'c':
				pcPokemon.push_back(new pokemon(4));
				mvprintw(0, 0, "Selected Charmander");
				delwin(pick);
				return;
			case 's':
				pcPokemon.push_back(new pokemon(7));
				mvprintw(0, 0, "Selected Squirtle");
				delwin(pick);
				return;
			default:
				mvwprintw(pick, 9, 20, "Enter a valid input (b/c/s)");
		}
	}
}

//sets world pointers to null and initializes first scene
void startWorld(){
	int i, j;
	for(i = 0; i < WORLD_SIZE; i++){
		for(j = 0; j < WORLD_SIZE; j++){
			world[i][j] = NULL;
		}
	}
	
	xCord = ORIGIN;
	yCord = ORIGIN;
	world[ORIGIN][ORIGIN] = (scene_t *) malloc(sizeof (scene_t));
	world[ORIGIN][ORIGIN]->n = -1;
	world[ORIGIN][ORIGIN]->s = -1;
	world[ORIGIN][ORIGIN]->w = -1;
	world[ORIGIN][ORIGIN]->e = -1;
	world[ORIGIN][ORIGIN]->x = ORIGIN;
	world[ORIGIN][ORIGIN]->y = ORIGIN;
	buildScene(world[ORIGIN][ORIGIN], -1, -1);
	initScreen();
	printScene(world[xCord][yCord]);
	pickPoke();
}

void placePlayer(scene_t *s, int x, int y){
	if(x <= 0 || x >= WIDTH - 1 || y <= 0 || y >= HEIGHT - 1){		
		while(1){
			x = (rand() % (WIDTH - 2)) + 1;
			y = (rand() % (HEIGHT - 2)) + 1;
	
			if(world[xCord][yCord]->scene[x][y] == '#'){	
				player[dim_x] = x;
				player[dim_y] = y;
				character_t *c = new character_t();
				c->type = (charType_t) PC;
				c->cost = 0;
				c->defeated = 0;
				c->hn = heap_insert(&(s->eventDriver), c);
				c->pos[dim_x] = x;
				c->pos[dim_y] = y;
				world[xCord][yCord]->charScene[x][y] = c;
				pcBag.revives = 3;
				pcBag.potions = 5;
				pcBag.pokeballs = 3;
				return;
			}
		}
	}else{
		player[dim_x] = x;
		player[dim_y] = y;
		character_t *c = new character_t();
		c->type = (charType_t) PC;
		c->cost = 0;
		c->defeated = 0;
		c->hn = heap_insert(&(s->eventDriver), c);
		c->pos[dim_x] = x;
		c->pos[dim_y] = y;
		world[xCord][yCord]->charScene[x][y] = c;
		pcBag.revives = 3;
		pcBag.potions = 5;
		pcBag.pokeballs = 3;
		return;
	}
}

//frees all allocated space for scenes
void endWorld(){
	int i, j;//, b, c;
	character_t *c;

	//FILE *f = fopen("debug.txt", "w");

	//fprintf(f, "Seed: %d\n", seed);


	std::vector<poke_class *>::iterator vi;
	std::vector<pokemon *>::iterator pi;

	for(pi = pcPokemon.begin(); pi != pcPokemon.end(); pi++){
		delete *pi;
	}
	for(vi = pokemon_datas.begin(); vi != pokemon_datas.end(); vi++){
		delete *vi;
	}
	for(vi = moves.begin(); vi != moves.end(); vi++){
		delete *vi;
	}
	for(vi = pokemon_moves.begin(); vi != pokemon_moves.end(); vi++){
		delete *vi;
	}
	for(vi = species.begin(); vi != species.end(); vi++){
		delete *vi;
	}
	for(vi = experiences.begin(); vi != experiences.end(); vi++){
		delete *vi;
	}
	for(vi = type_names.begin(); vi != type_names.end(); vi++){
		delete *vi;
	}
	for(vi = pokemon_stats.begin(); vi != pokemon_stats.end(); vi++){
		delete *vi;
	}
	for(vi = stats.begin(); vi != stats.end(); vi++){
		delete *vi;
	}
	for(vi = pokemon_types.begin(); vi != pokemon_types.end(); vi++){
		delete *vi;
	}

	for(i = 0; i < WORLD_SIZE; i++){
		for(j = 0; j < WORLD_SIZE; j++){
			if(world[i][j] != NULL){
				while((c = (character_t*) heap_remove_min(&(world[i][j]->eventDriver)))){
					for(pi = c->pets.begin(); pi != c->pets.end(); pi++){
						delete *pi;
					}
					c->hn = NULL;
					delete c;
				}
				
				/*for(b = 0; b < WIDTH; b++){
					for(c = 0; c < HEIGHT; c++){
						if(world[i][j]->charScene[b][c] != NULL){
							fprintf(f, "xCord: %d, yCord: %d, x: %d, y: %d\n", i, j, b, c);
							free(world[i][j]->charScene[b][c]);
							world[i][j]->charScene[b][c] = NULL;
						}
					}
				}*/
				heap_delete(&(world[i][j]->eventDriver));
				free(world[i][j]);
			}
		}
	}
	//fclose(f);
}

//uses xCord and yCord to allocate memory and generate a 
//new scene if one doesn't exist, will print generated or pre-existing scene
void switchScene(int x, int y){
	if(world[xCord][yCord] == NULL){
		world[xCord][yCord] = (scene_t *)malloc(sizeof(scene_t));
		
		//implements checking for neighboring gates
		world[xCord][yCord]->w = (xCord > 0 && world[xCord - 1][yCord] != NULL)? world[xCord - 1][yCord]->e: -1;
		world[xCord][yCord]->e = (xCord < WORLD_SIZE - 1 && world[xCord + 1][yCord] != NULL)? world[xCord + 1][yCord]->w: -1;
		world[xCord][yCord]->s = (yCord > 0 && world[xCord][yCord - 1] != NULL)? world[xCord][yCord - 1]->n: -1;
		world[xCord][yCord]->n = (yCord < WORLD_SIZE - 1 && world[xCord][yCord + 1] != NULL)? world[xCord][yCord + 1]->s: -1;
		
		world[xCord][yCord]->x = xCord;
		world[xCord][yCord]->y = yCord;

		buildScene(world[xCord][yCord], x, y);
	}else{
		findPC(world[xCord][yCord], x, y);
	}
}

void findPC(scene_t *s, int x, int y){
	int i, j;
	
	if(x < 1 || x >= WIDTH - 1 || y < 1 || y >= HEIGHT - 1){
		for(i = 1; i < WIDTH - 1; i++){
			for(j = 1; j < HEIGHT - 1; j++){
				if(s->charScene[i][j] != NULL &&
					s->charScene[i][j]->type == PC){
					
					player[dim_y] = j;
					player[dim_x] = i;
					return;
				}
			}
		}
	}

	for(i = 1; i < WIDTH - 1; i++){
		for(j = 1; j < HEIGHT - 1; j++){
			if(s->charScene[i][j] != NULL &&
				s->charScene[i][j]->type == PC){
				
				player[dim_y] = y;
				player[dim_x] = x;
				
				if(i == x && j == y){
					return;
				}

				character_t *c = s->charScene[i][j];
				s->charScene[i][j] = s->charScene[x][y];
				s->charScene[x][y] = c;
				c->pos[dim_x] = x;
				c->pos[dim_y] = y;
				dijkstra(hikerMap, Hiker);
				dijkstra(rivalMap, Rival);
				return;
			}
		}
	}
}

void givePokemon(character *c){
	pokemon *p = new pokemon();
	(c->pets).push_back(p);
	while(c->pets.size() < 6){
		if(rand() % CHNC < 40) break;
		p = new pokemon();
		(c->pets).push_back(p);
	}
}

void placeNPCs(int numNPCs, scene_t *s){
	character_t *character;
	int kind, x, y;
	
	while(numNPCs > 0){
		kind = rand() % 6;
		while(1){
			x = (rand() % (WIDTH - 2)) + 1;
			y = (rand() % (HEIGHT - 2)) + 1;
			
			if(kind == 0 && hikerMap[x][y].cost != INT_MAX && 
					s->charScene[x][y] == NULL){
				character = new character_t();
				character->type = (charType_t) Hiker;
				character->dir = static_cast<direction_t>(rand() % 4);
				character->cost = 0;
				character->pos[dim_x] = x;
				character->pos[dim_y] = y;
				s->charScene[x][y] = character;
				character->defeated = 0;
				character->hn = heap_insert(&(s->eventDriver), character);
				givePokemon(character);
				numNPCs--;
				break;
			}else if(kind != 0 && rivalMap[x][y].cost != INT_MAX &&
					s->charScene[x][y] == NULL){
				character = new character_t();
				character->cost = 0;
				switch(kind){
					case 1:
						character->type = Rival;
						character->dir = static_cast<direction_t>(rand() % 4);
						break;
					case 2:
						character->type = Pacer;
						character->dir = static_cast<direction_t>(rand() % 4);
						break;
					case 3:
						character->type = Wanderer;
						character->dir = static_cast<direction_t>(rand() % 4);
						break;
					case 4:
						character->type = Sentry;
						break;
					case 5:
						character->type = Explorer;
						character->dir = static_cast<direction_t>(rand() % 4);
						break;
					default:
						break;
				}
			
				character->pos[dim_x] = x;
				character->pos[dim_y] = y;
				s->charScene[x][y] = character;
				character->defeated = 0;
				character->hn = heap_insert(&(s->eventDriver), character);
				givePokemon(character);
				numNPCs--;
				break;	
			}
		}	
	}
}

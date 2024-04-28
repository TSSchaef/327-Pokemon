#include <cstdlib>
#include "pokemon.h"

#define ORIGIN 200

int useAbility(ability *a, pokemon *attacker, pokemon *defender){
	if(a->getPower() == INT_MAX) return 1;

	double amount = attacker->getLevel();
	amount *= 2;
	amount /= 5;
	amount += 2;
	amount *= a->getPower();
	amount *= (attacker->getCurrAttack() / defender->getCurrDefense());
	amount /= 50;
	amount += 2;
	
	double critDamage = (rand() % 256) < (attacker->getBaseSpeed() / 2) ? 1.5 : 1.0;

	amount *= critDamage;
	amount *= ((rand() % 16) + 85) / 100.0;

	if(a->getType() == attacker->getType()) amount *= 1.5;

	if(amount < 1) return 1;
	
	return (int) amount;
}

ability::ability(){
	id = 165;
	identifier = std::string("struggle");
	type = std::string("Normal");
	power = 50;
	pp = 1;
	accuracy = INT_MAX;
	priority = 0;
}
ability::ability(int id){
	std::vector<poke_class *>::iterator vi;
	int typeId;
	for(vi = moves.begin(); vi != moves.end(); vi++){
		if(((move_t *) *vi)->getId() == id){
			//get/set values
			this->id = id;
			identifier = ((move_t *) *vi)->getIdentifier();
			typeId = ((move_t *) *vi)->getTypeID();
			power = ((move_t *) *vi)->getPower();
			pp = ((move_t *) *vi)->getPP();
			accuracy = ((move_t *) *vi)->getAccuracy();
			priority = ((move_t *) *vi)->getPriority();
			break;
		}
	}
	for(vi = type_names.begin(); vi != type_names.end(); vi++){
		if(((type_name *) *vi)->getTypeId() == typeId){
			type = ((type_name *) *vi)->getName();
			break;
		}
	}	
}
ability::~ability(){}

std::string ability::getIdentifier() const{ return identifier;}
std::string ability::getType() const{ return type;}
int ability::getId() const{ return id;}
int ability::getPower() const{ return power == INT_MAX ? 1 : power;}
int ability::getPP() const{ return pp;}
int ability::getAccuracy() const{ return accuracy == INT_MAX ? 100 : accuracy;}
int ability::getPriority() const{ return priority;}


pokemon::pokemon() : pokemon( (id = (rand() % 1092) + 1) > 898? id += 9102 : id){}
pokemon::pokemon(int id){
	std::vector<poke_class *>::iterator vi;
	for(vi = pokemon_datas.begin(); vi != pokemon_datas.end(); vi++){
		if(((pokemon_data *) *vi)->getId() == id){
			//get data
			this->id = id;
			species_id = ((pokemon_data *) *vi)->getSpeciesID();
			height = ((pokemon_data *) *vi)->getHeight();
			weight = ((pokemon_data *) *vi)->getWeight();
			currXP = ((pokemon_data *) *vi)->getBaseExperience();
			order = ((pokemon_data *) *vi)->getOrder();
			identifier = ((pokemon_data *) *vi)->getIdentifier();
			break;
		}
	}
	for(vi = species.begin(); vi != species.end(); vi++){
		if(((pokemon_species *) *vi)->getId() == species_id){
			genus = ((pokemon_species *) *vi)->getIdentifier();
			capture_rate = ((pokemon_species *) *vi)->getCaptureRate();		
			growth_rate_id = ((pokemon_species *) *vi)->getGrowthRateID();
			int gr = ((pokemon_species *) *vi)->getGenderRate();
			if(gr < 0 || gr == INT_MAX) gender = std::string("None");
			else{
				if((rand() % 8) < gr){
					gender = "Female";
				}else{
					gender = "Male";
				}
			}	
			break;
		}
	}
	//gets manhattan distance from spawn
	int distance = ((xCord - ORIGIN) < 0? -1 * (xCord - ORIGIN) : (xCord - ORIGIN)) + ((yCord - ORIGIN) < 0? -1 * (yCord - ORIGIN) : (yCord - ORIGIN));
	if(distance <= 200){
		level = (rand() % ((distance / 2) + 1)) + 1;
	}else{
		level = 100 - (rand() % (((400 - distance) / 200) + 1));
	}

	for(vi = experiences.begin(); vi != experiences.end(); vi++){
		if(growth_rate_id < 1 || growth_rate_id > 6) growth_rate_id = 1;
		if(((experience *) *vi)->getGrowthRateId() == growth_rate_id && (((experience *) *vi)->getLevel() > level || ((experience *) *vi)->getLevel() == 100)){
			currXP = ((experience *) *(vi - 1))->getExperience();
			experienceLvlUp = ((experience *) *vi)->getExperience();
			break;
		}
	}
	int type_id;
	for(vi = pokemon_types.begin(); vi != pokemon_types.end(); vi++){
		if(((pokemon_type *) *vi)->getPokemonId() == id){
			type_id = ((pokemon_type *) *vi)->getTypeID();
			break;
		}
	}
	for(vi = type_names.begin(); vi != type_names.end(); vi++){
		if(((type_name *) *vi)->getTypeId() == type_id){
			type = ((type_name *) *vi)->getName();
			break;
		}
	}
	for(vi = pokemon_stats.begin(); vi != pokemon_stats.end(); vi++){
		if(((pokemon_stat *) *vi)->getPokemonId() == id){
			switch(((pokemon_stat *) *vi)->getStatID()){
				case 1:
					hp = ((pokemon_stat *) *vi)->getBaseStat();
					break;
				case 2:
					attack = ((pokemon_stat *) *vi)->getBaseStat();
					break;
				case 3:
					defense = ((pokemon_stat *) *vi)->getBaseStat();
					break;
				case 4:
					specialAttack = ((pokemon_stat *) *vi)->getBaseStat();
					break;
				case 5:
					specialDefense = ((pokemon_stat *) *vi)->getBaseStat();
					break;
				case 6:
					speed = ((pokemon_stat *) *vi)->getBaseStat();
					goto breakloop;
			}
		}
	}
	breakloop:
	
	for(vi = pokemon_moves.begin(); vi != pokemon_moves.end(); vi++){
		if(((pokemon_move *) *vi)->getPokeId() == species_id){
			version_id = ((pokemon_move *) *vi)->getVersionID();

			while(version_id == ((pokemon_move *) *vi)->getVersionID()){
				if(((pokemon_move *) *vi)->getLevel() < level && ((pokemon_move *)*vi)->getMoveMethodID() == 1){
					abilities.push_back(new ability(((pokemon_move *) *vi)->getMoveID()));
				}
				vi++;
			}
			break;
		}
	}
	//adds struggle if pokemon has no abilities
	if(abilities.size() == 0) abilities.push_back(new ability());
	//removes abilities until pokemon only has 2
	while(abilities.size() > 2){
		std::vector<ability *>::iterator ai = abilities.begin() + (rand() % abilities.size());
		delete *ai;
		abilities.erase(ai);
	}
	//IV values and shiny chance
	color = (rand() % 8192) == 0? std::string("Shiny") : "Normal";
	hpIV = rand() % 16;
	attackIV = rand() % 16;
	defenseIV = rand() % 16;
	specialAttackIV = rand() % 16;
	specialDefenseIV = rand() % 16;
	speedIV = rand() % 16;

	hpCurr = getTotalHp();
	attackCurr = getTotalAttack();
	defenseCurr = getTotalDefense();
	specialAttackCurr = getTotalSpecialAttack();
	specialDefenseCurr = getTotalSpecialDefense();
	speedCurr = getTotalSpeed();

	is_alive = true;
}

pokemon::~pokemon(){
	std::vector<ability *>::iterator ai;
	for(ai = abilities.begin(); ai != abilities.end(); ai++){
		delete *ai;
	}
}

void pokemon::levelUp(){}
void pokemon::setName(std::string name){this->name = name;}
void pokemon::setName(const char* name){this->name = std::string(name);}

void pokemon::addAbility(ability a){
	abilities.push_back(&a);
}

void pokemon::receiveDamage(int amount){
	hpCurr -= amount;
	if(hpCurr < 0) hpCurr = 0;
	is_alive = hpCurr > 0;
}

//TO DO:returns true if leveled up
bool pokemon::addXP(int amount){return false;}
bool pokemon::revive(){
	if(!is_alive){
		is_alive = true;
		hpCurr = this->getTotalHp()/2;
		return true;
	}else{
		return false;
	}	
}
bool pokemon::heal(){
	if(hpCurr < this->getTotalHp() && is_alive){
		hpCurr += 20;
		if(hpCurr > this->getTotalHp()) hpCurr = this->getTotalHp();
		return true;
	}else{
		return false;
	}
}
void pokemon::center(){
	is_alive = true;
	hpCurr = this->getTotalHp();
}	
	
int pokemon::sellValue() const{ return 0;}
std::string pokemon::getName() const{return name;}
std::string pokemon::getSpecies() const{return genus;}
std::string pokemon::getType() const{ return type;}
std::string pokemon::getIdentifier() const{ return identifier;}
std::string pokemon::getGender() const{ return gender;}
std::string pokemon::getColor() const{ return color;}
bool pokemon::isAlive() const{ return is_alive;} 
int pokemon::getId() const{ return id;}
int pokemon::getHeight() const{ return height;}
int pokemon::getWeight() const{ return weight;}
int pokemon::getOrder() const{ return order;}
int pokemon::getLevel() const{ return level;}
int pokemon::getCurrHp() const{ return hpCurr;}
int pokemon::getTotalHp() const{ return (((hp + hpIV) * 2 * level) / 100) + level + 10;}
int pokemon::getCurrAttack() const{ return attackCurr;}
int pokemon::getTotalAttack() const{ return (((attack + attackIV) * 2 * level) / 100) + 5;}
int pokemon::getCurrDefense() const{ return defenseCurr;}
int pokemon::getTotalDefense() const{ return (((defense + defenseIV) * 2 * level) / 100) + 5;}
int pokemon::getTotalSpeed() const{ return (((speed + speedIV) * 2 * level) / 100) + 5;}
int pokemon::getCurrSpecialAttack() const{ return specialAttackCurr;}
int pokemon::getTotalSpecialAttack() const{ return (((specialAttack + specialAttackIV) * 2 * level) / 100) + 5;}
int pokemon::getCurrSpecialDefense() const { return specialDefenseCurr;}
int pokemon::getTotalSpecialDefense() const { return (((specialDefense + specialDefenseIV) * 2 * level) / 100) + 5;}
int pokemon::getCurrSpeed() const{ return speedCurr;}
int pokemon::getBaseSpeed() const{ return speed;}
int pokemon::getCaptureRate() const{ return capture_rate;}
int pokemon::getCurrEvasion() const{ return evasionCurr;}
int pokemon::getTotalEvasion() const{ return evasion;}
int pokemon::getCurrXP() const {return currXP;}
int pokemon::getXPLvlUp() const {return experienceLvlUp;}
std::vector<ability *> pokemon::getAbilities() const{ return abilities;}

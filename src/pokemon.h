#ifndef POKEMON_H
#define POKEMON_H

#include <vector>

#include "pokeClasses.h"


extern int xCord, yCord;
extern std::vector<poke_class *> pokemon_datas, moves, pokemon_moves, species, experiences, type_names, pokemon_stats, stats, pokemon_types; 

//returns amount of damage done !!does not check pokemon has ability it is using

/*class effect{
	
};*/

class ability{
private:
	int id, power, pp, accuracy, priority;
	std::string identifier, type;
	//effect e;
	//bool has_effect;
public:
	ability();
	ability(int id);
	~ability();
	
	std::string getIdentifier() const;
	std::string getType() const;
	int getId() const;
	int getPower() const;
	int getPP() const;
	int getAccuracy() const;
	int getPriority() const;
	//effect getEffect();
	//bool hasEffect();
};

class pokemon{
private:
	int id, species_id, version_id, height, weight, currXP, order, level;
	std::string identifier, genus, type, name, gender, color;
	int experienceLvlUp, capture_rate, growth_rate_id;
	//base values	
	int hp, attack, defense, specialAttack, specialDefense, speed, evasion; 
	//individual values
	int hpIV, attackIV, defenseIV, specialAttackIV, specialDefenseIV, speedIV;
	//current values
	int hpCurr, attackCurr, defenseCurr, specialAttackCurr, specialDefenseCurr, speedCurr, evasionCurr;
	bool is_alive;
	//effects currently on this pokemon
	//std::vector<effect *> effects;
	//abilities/moves this pokemon has
	std::vector<ability *> abilities;

	void levelUp();
public:
	pokemon();
	pokemon(int id);
	~pokemon();
	void setName(std::string name);
	void setName(const char* name);
	//void addEffect(effect e);
	void addAbility(ability a);
	//void removeEffect(effect e);
		
	void receiveDamage(int amount);
	
	//returns true if leveled up
	bool addXP(int amount);
	//returns true if item benifitted pokemon
	bool revive();
	bool heal();
	
	void center();	
	
	int sellValue() const;
	std::string getName() const;
	std::string getSpecies() const;
	std::string getType() const;
	std::string getIdentifier() const;
	std::string getGender() const;
	std::string getColor() const;
	bool isAlive() const; 
	int getId() const;
	int getHeight() const;
	int getWeight() const;
	int getOrder() const;
	int getLevel() const;
	int getCurrHp() const;
	int getTotalHp() const;
	int getCurrAttack() const;
	int getTotalAttack() const;
	int getCurrDefense() const;
	int getTotalDefense() const;
	int getCurrSpecialAttack() const;
	int getTotalSpecialAttack() const;
	int getCurrSpecialDefense() const;
	int getTotalSpecialDefense() const;
	int getTotalSpeed() const;
	int getBaseSpeed() const;
	int getCurrSpeed() const;
	int getCaptureRate() const;
	int getCurrEvasion() const;
	int getTotalEvasion() const;
	int getCurrXP() const;
	int getXPLvlUp() const;
	//std::vector<effect *> getEffects() const;
	std::vector<ability *> getAbilities() const;
};

int useAbility(ability *a, pokemon *attacker, pokemon *defender);

#endif

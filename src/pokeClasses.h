#ifndef POKECLASSES_H
#define POKECLASSES_H

/*
 *	Classes used to stored data about pokemon as it is read 
 *	directly in from csv files. These hold raw data that will
 *	generate actual pokemon for the game later.
 */

#include <climits>
#include <iostream>
#include <string>

class poke_class{
public:
	virtual ~poke_class() {}
	virtual std::ostream &print(std::ostream &o) const = 0;
};

std::ostream &operator<<(std::ostream &o, const poke_class &p);

class pokemon_data : public poke_class{
private:
	int id;
	std::string identifier;
	int species_id, height, weight, base_experience, order, is_default;
public:
	void setId(int num);
	void setIdentifier(std::string name);
	void setSpeciesID(int id);
	void setHeight(int h);
	void setWeight(int w);
	void setBaseExperience(int exp);
	void setOrder(int o);
	void setIsDefault(int d);
	int getId();
	std::string getIdentifier();
	int getSpeciesID();
	int getHeight();
	int getWeight();
	int getBaseExperience();
	int getOrder();
	std::ostream &print(std::ostream &o) const;
};

class pokemon_move : public poke_class{
private:
	int poke_id, version_id, move_id, move_method_id, level, order;
public:
	void setPokeId(int num);
	void setVersionID(int id);
	void setMoveID(int h);
	void setMoveMethodID(int w);
	void setLevel(int exp);
	void setOrder(int o);
	int getPokeId();
	int getMoveID();
	int getVersionID();
	int getMoveMethodID();
	int getLevel();
	std::ostream &print(std::ostream &o) const;
}; 

class pokemon_stat : public poke_class{
private:
	int pokemon_id, stat_id, base_stat, effort;
public:
	void setPokemonId(int num);
	void setStatID(int id);
	void setBaseStat(int h);
	void setEffort(int h);
	int getPokemonId();
	int getStatID();
	int getBaseStat();
	std::ostream &print(std::ostream &o) const;
};

class stats : public poke_class{
private:
	int id, damage_class_id;
	std::string identifier;
	int is_battle_only, game_index;
public:
	void setId(int num);
	void setDamageClassID(int id);
	void setIsBattleOnly(int h);
	void setGameIndex(int h);
	void setIdentifier(std::string name);
	std::ostream &print(std::ostream &o) const;
};

class pokemon_type : public poke_class{
private:
	int pokemon_id, type_id, slot;
public:
	void setPokemonId(int num);
	void setTypeID(int id);
	void setSlot(int h);
	int getPokemonId();
	int getTypeID();
	std::ostream &print(std::ostream &o) const;
};

class experience : public poke_class{
private:
	int growth_rate_id, level, experience;
public:
	void setGrowthRateId(int num);
	void setLevel(int id);
	void setExperience(int h);
	int getGrowthRateId();
	int getLevel();
	int getExperience();
	std::ostream &print(std::ostream &o) const;
};

class type_name : public poke_class{
private:
	int type_id, local_language_id;
       	std::string name;
public:
	void setTypeId(int num);
	void setLocalLanguageID(int id);
	void setName(std::string h);
	int getTypeId();
	std::string getName();
	std::ostream &print(std::ostream &o) const;
};

typedef class move : public poke_class {
private:
	int id;
	std::string identifier;
	int generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_contest_effect_id;
public:
	void setId(int num);
	void setIdentifier(std::string name);
	void setGenerationID(int num);
	void setTypeID(int num);
	void setPower(int num);
	void setPP(int num);
	void setAccuracy(int num);
	void setPriority(int num);
	void setTargetID(int num);
	void setDamageClassID(int num);
	void setEffectID(int num);
	void setEffectChance(int num);
	void setContestTypeID(int num);
	void setContestEffectID(int num);
	void setSuperContestEffectID(int num);
	int getId();
	std::string getIdentifier();
	int getTypeID();
	int getPower();
	int getPP();
	int getAccuracy();
	int getPriority();
	std::ostream &print(std::ostream &o) const;
}  move_t;

class pokemon_species : public poke_class{
private:
	int id;
	std::string identifier;
	int generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
public:
	void setId(int num);
	void setIdentifier(std::string name);
	void setGenerationID(int num);
	void setEvolvesFromSpeciesID(int num);
	void setEvolutionChainID(int num);
	void setColorID(int num);
	void setShapeID(int num);
	void setHabitatID(int num);
	void setGenderRate(int num);
	void setCaptureRate(int num);
	void setBaseHappiness(int num);
	void setIsBaby(int num);
	void setHatchCounter(int num);
	void setHasGenderDifferences(int num);
	void setGrowthRateID(int num);
	void setFormsSwitchable(int num);
	void setIsLegendary(int num);
	void setIsMythical(int num);
	void setOrder(int num);
	void setConquestOrder(int num);
	int getId();
	std::string getIdentifier();
	int getGenderRate();
	int getCaptureRate();
	int getGrowthRateID();
	std::ostream &print(std::ostream &o) const;
};

#endif

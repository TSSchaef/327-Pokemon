#include "pokeClasses.h"

std::ostream &operator<<(std::ostream &o, const poke_class &p){
	return p.print(o);
}

void pokemon_data::setId(int num){
	id = num;
}
void pokemon_data::setIdentifier(std::string name){
	identifier = name;
}
void pokemon_data::setSpeciesID(int id){
	species_id = id;
}
void pokemon_data::setHeight(int h){
	height = h;
}
void pokemon_data::setWeight(int w){
	weight = w;
}
void pokemon_data::setBaseExperience(int exp){
	base_experience = exp;
}
void pokemon_data::setOrder(int o){
	order = o;
}
void pokemon_data::setIsDefault(int d){
	is_default = d;
}
int pokemon_data::getId(){ return id;}
std::string pokemon_data::getIdentifier(){ return identifier;}
int pokemon_data::getHeight(){return height;}
int pokemon_data::getWeight(){ return weight;}
int pokemon_data::getBaseExperience(){return base_experience;}
int pokemon_data::getOrder(){return order;}
int pokemon_data::getSpeciesID(){return species_id;}
std::ostream &pokemon_data::print(std::ostream &o) const{
	return o << id << "," << identifier << "," << ((species_id == INT_MAX)? std::string(): std::to_string(species_id)) << "," << ((height == INT_MAX)? std::string(): std::to_string(height)) << "," << ((weight == INT_MAX)? std::string(): std::to_string(weight)) << "," << ((base_experience == INT_MAX)? std::string(): std::to_string(base_experience)) << "," << ((order == INT_MAX)? std::string(): std::to_string(order)) << "," << ((is_default == INT_MAX)? std::string(): std::to_string(is_default)); 
}

void pokemon_move::setPokeId(int num){
	poke_id = num;
}
void pokemon_move::setVersionID(int id){
	version_id = id;
}
void pokemon_move::setMoveID(int h){
	move_id = h;
}
void pokemon_move::setMoveMethodID(int w){
	move_method_id = w;
}
void pokemon_move::setLevel(int exp){
	level = exp;
}
void pokemon_move::setOrder(int o){
	order = o;
}
int pokemon_move::getMoveMethodID(){ return move_method_id;}
int pokemon_move::getPokeId(){ return poke_id;}
int pokemon_move::getMoveID(){ return move_id;}
int pokemon_move::getLevel(){ return level;}
int pokemon_move::getVersionID(){ return version_id;}
std::ostream &pokemon_move::print(std::ostream &o) const{
	return o << poke_id << "," << ((version_id == INT_MAX)? std::string(): std::to_string(version_id)) << "," << ((move_id == INT_MAX)? std::string(): std::to_string(move_id)) << "," << ((move_method_id == INT_MAX)? std::string(): std::to_string(move_method_id)) << "," << ((level == INT_MAX)? std::string(): std::to_string(level)) << "," << ((order == INT_MAX)? std::string(): std::to_string(order)); 
}



void pokemon_stat::setPokemonId(int num){
	pokemon_id = num;
}
void pokemon_stat::setStatID(int id){
	stat_id = id;
}
void pokemon_stat::setBaseStat(int h){
	base_stat = h;
}
void pokemon_stat::setEffort(int h){
	effort = h;
}
int pokemon_stat::getPokemonId(){return pokemon_id;}
int pokemon_stat::getStatID(){ return stat_id;}
int pokemon_stat::getBaseStat(){ return base_stat;}
std::ostream &pokemon_stat::print(std::ostream &o) const{
	return o << pokemon_id << "," << ((stat_id == INT_MAX)? std::string(): std::to_string(stat_id)) << "," << ((base_stat == INT_MAX)? std::string(): std::to_string(base_stat)) << "," << ((effort == INT_MAX)? std::string() : std::to_string(effort)); 
}


void stats::setId(int num){
	id = num;
}
void stats::setDamageClassID(int id){
	damage_class_id = id;
}
void stats::setIsBattleOnly(int h){
	is_battle_only = h;
}
void stats::setGameIndex(int h){
	game_index = h;
}
void stats::setIdentifier(std::string name){
	identifier = name;
}
std::ostream &stats::print(std::ostream &o) const{
	return o << id << "," << ((damage_class_id == INT_MAX)? std::string(): std::to_string(damage_class_id)) << "," << identifier << "," << ((is_battle_only == INT_MAX)? std::string(): std::to_string(is_battle_only)) << "," << ((game_index == INT_MAX)? std::string() : std::to_string(game_index)); 
}


void pokemon_type::setPokemonId(int num){
	pokemon_id = num;
}
void pokemon_type::setTypeID(int id){
	type_id = id;
}
void pokemon_type::setSlot(int h){
	slot = h;
}
int pokemon_type::getPokemonId(){return pokemon_id;}
int pokemon_type::getTypeID(){ return type_id;}
std::ostream &pokemon_type::print(std::ostream &o) const{
	return o << pokemon_id << "," << ((type_id == INT_MAX)? std::string(): std::to_string(type_id)) << "," << ((slot == INT_MAX)? std::string(): std::to_string(slot)); 
}


void experience::setGrowthRateId(int num){
	growth_rate_id = num;
}
void experience::setLevel(int id){
	level = id;
}
void experience::setExperience(int h){
	experience = h;
}
int experience::getGrowthRateId(){return growth_rate_id;}
int experience::getLevel(){return level;}
int experience::getExperience(){return experience;}
std::ostream &experience::print(std::ostream &o) const{
	return o << growth_rate_id << "," << ((level == INT_MAX)? std::string(): std::to_string(level)) << "," << ((experience == INT_MAX)? std::string(): std::to_string(experience)); 
}


void type_name::setTypeId(int num){
	type_id = num;
}
void type_name::setLocalLanguageID(int id){
	local_language_id = id;
}
void type_name::setName(std::string h){
	name = h;
}
int type_name::getTypeId(){ return type_id;}
std::string type_name::getName(){return name;}
std::ostream &type_name::print(std::ostream &o) const{
	return o << ((type_id == INT_MAX)? std::string(): std::to_string(type_id)) << "," << ((local_language_id == INT_MAX)? std::string(): std::to_string(local_language_id)) << "," << name; 
}

void move_t::setId(int num){
	id = num;
}
void move_t::setIdentifier(std::string name){
	identifier = name;
}
void move_t::setGenerationID(int num){
	generation_id = num;
}
void move_t::setTypeID(int num){
	type_id = num;
}
void move_t::setPower(int num){
	power = num;
}
void move_t::setPP(int num){
	pp = num;
}
void move_t::setAccuracy(int num){
	accuracy = num;
}
void move_t::setPriority(int num){
	priority = num;
}
void move_t::setTargetID(int num){
	target_id = num;
}
void move_t::setDamageClassID(int num){
	damage_class_id = num;
}
void move_t::setEffectID(int num){
	effect_id = num;
}
void move_t::setEffectChance(int num){
	effect_chance = num;
}
void move_t::setContestTypeID(int num){
	contest_type_id = num;
}
void move_t::setContestEffectID(int num){
	contest_effect_id = num;
}
void move_t::setSuperContestEffectID(int num){
	super_contest_effect_id = num;
}
int move_t::getId(){ return id;}
std::string move_t::getIdentifier(){ return identifier;}
int move_t::getTypeID(){return type_id;}
int move_t::getPower(){ return power; }
int move_t::getPP(){return pp;}
int move_t::getAccuracy(){ return accuracy;}
int move_t::getPriority(){ return priority;}
std::ostream &move_t::print(std::ostream &o) const{
	return o << id << "," << identifier << "," << ((generation_id == INT_MAX)? std::string(): std::to_string(generation_id)) << "," << ((type_id == INT_MAX)? std::string(): std::to_string(type_id)) << "," << ((power == INT_MAX)? std::string(): std::to_string(power)) << "," << ((pp == INT_MAX)? std::string(): std::to_string(pp)) << "," << ((accuracy == INT_MAX)? std::string(): std::to_string(accuracy)) << "," << ((priority == INT_MAX)? std::string(): std::to_string(priority)) << "," << ((target_id == INT_MAX)? std::string(): std::to_string(target_id)) << "," << ((damage_class_id == INT_MAX)? std::string(): std::to_string(damage_class_id)) << "," << ((effect_id == INT_MAX)? std::string(): std::to_string(effect_id)) << "," << ((effect_chance == INT_MAX)? std::string(): std::to_string(effect_chance)) << "," << ((contest_type_id == INT_MAX)? std::string(): std::to_string(contest_type_id)) << "," << ((contest_effect_id == INT_MAX)? std::string(): std::to_string(contest_effect_id)) << "," << ((super_contest_effect_id == INT_MAX)? std::string(): std::to_string(super_contest_effect_id));
}


void pokemon_species::setId(int num){
	id = num;
}
void pokemon_species::setIdentifier(std::string name){
	identifier = name;
}
void pokemon_species::setGenerationID(int num){
	generation_id = num;
}
void pokemon_species::setEvolvesFromSpeciesID(int num){
	evolves_from_species_id = num;
}
void pokemon_species::setEvolutionChainID(int num){
	evolution_chain_id = num;
}
void pokemon_species::setColorID(int num){
	color_id = num;
}
void pokemon_species::setShapeID(int num){
	shape_id = num;
}
void pokemon_species::setHabitatID(int num){
	habitat_id = num;
}
void pokemon_species::setGenderRate(int num){
	gender_rate = num;
}
void pokemon_species::setCaptureRate(int num){
	capture_rate = num;
}
void pokemon_species::setBaseHappiness(int num){
	base_happiness = num;
}
void pokemon_species::setIsBaby(int num){
	is_baby = num;
}
void pokemon_species::setHatchCounter(int num){
	hatch_counter = num;
}
void pokemon_species::setHasGenderDifferences(int num){
	has_gender_differences = num;
}
void pokemon_species::setGrowthRateID(int num){
	growth_rate_id = num;
}
void pokemon_species::setFormsSwitchable(int num){
	forms_switchable = num;
}
void pokemon_species::setIsLegendary(int num){
	is_legendary = num;
}
void pokemon_species::setIsMythical(int num){
	is_mythical = num;
}
void pokemon_species::setOrder(int num){
	order = num;
}
void pokemon_species::setConquestOrder(int num){
	conquest_order = num;
}
int pokemon_species::getId(){return id;}
std::string pokemon_species::getIdentifier(){ return identifier;}
int pokemon_species::getGenderRate(){return gender_rate;}
int pokemon_species::getCaptureRate(){return capture_rate;}
int pokemon_species::getGrowthRateID(){return growth_rate_id;}
std::ostream &pokemon_species::print(std::ostream &o) const{
	return o << id << "," << identifier << "," << ((generation_id == INT_MAX)? std::string(): std::to_string(generation_id)) << "," << ((evolves_from_species_id == INT_MAX)? std::string(): std::to_string(evolves_from_species_id)) << "," << ((evolution_chain_id == INT_MAX)? std::string(): std::to_string(evolution_chain_id)) << "," << ((color_id == INT_MAX)? std::string(): std::to_string(color_id)) << "," << ((shape_id == INT_MAX)? std::string(): std::to_string(shape_id)) << "," << ((habitat_id == INT_MAX)? std::string(): std::to_string(habitat_id)) << "," << ((gender_rate == INT_MAX)? std::string(): std::to_string(gender_rate)) << "," << ((capture_rate == INT_MAX)? std::string(): std::to_string(capture_rate)) << "," << ((base_happiness == INT_MAX)? std::string(): std::to_string(base_happiness)) << "," << ((is_baby == INT_MAX)? std::string(): std::to_string(is_baby)) << "," << ((hatch_counter == INT_MAX)? std::string(): std::to_string(hatch_counter)) << "," << ((has_gender_differences == INT_MAX)? std::string(): std::to_string(has_gender_differences)) << "," << ((growth_rate_id == INT_MAX)? std::string(): std::to_string(growth_rate_id)) << "," << ((forms_switchable == INT_MAX)? std::string() : std::to_string(forms_switchable)) << "," << ((is_legendary == INT_MAX)? std::string() : std::to_string(is_legendary)) << "," << ((is_mythical == INT_MAX)? std::string() : std::to_string(is_mythical)) << "," << ((order == INT_MAX)? std::string() : std::to_string(order)) << "," << ((conquest_order == INT_MAX)? std::string() : std::to_string(conquest_order));
}

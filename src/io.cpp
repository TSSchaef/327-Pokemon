#include "io.h"

#define EMPTY INT_MAX

std::string *findDB(){
	struct stat sb;
	//my storage choice
	if(stat("pokedex/data/csv", &sb) == 0){
		return new std::string("pokedex/data/csv");
	//students on pyrite
	}else if(stat("../../../../share/cs327/pokedex/pokedex/data/csv", &sb) == 0){
		return new std::string("../../../../share/cs327/pokedex/pokedex/data/csv");
	//admin on pyrite
	}else if(stat("/share/cs327/pokedex/pokedex/data/csv", &sb) == 0){
		return new std::string("/share/cs327/pokedex/pokedex/data/csv");
	//db in home directory
	} else if(stat(strcat(getenv("HOME"),"/.poke327/pokedex/pokedex/data/csv"), &sb) == 0){
		return new std::string(strcat(getenv("HOME"),"/.poke327/pokedex/pokedex/data/csv"));
	}else {	
		return NULL;
	}
}


std::vector<poke_class *> scanStats(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/stats.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		stats *p = new stats();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setId(stoi(s));
		} else{
			file.get();
			p->setId(EMPTY);	
		}
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setDamageClassID(stoi(s));
		} else{
			file.get();
			p->setDamageClassID(EMPTY);	
		}
		
		std::getline(file, s, ',');
		p->setIdentifier(s);

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setIsBattleOnly(stoi(s));
		} else{
			file.get();
			p->setIsBattleOnly(EMPTY);	
		}
		
		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setGameIndex(stoi(s));
		} else{
			file.get();
			p->setGameIndex(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}


std::vector<poke_class *> scanPokeTypes(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/pokemon_types.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		pokemon_type *p = new pokemon_type();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setPokemonId(stoi(s));
		} else{
			file.get();
			p->setPokemonId(EMPTY);	
		}
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setTypeID(stoi(s));
		} else{
			file.get();
			p->setTypeID(EMPTY);	
		}
		
		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setSlot(stoi(s));
		} else{
			file.get();
			p->setSlot(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}


std::vector<poke_class *> scanPokeStats(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/pokemon_stats.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		pokemon_stat *p = new pokemon_stat();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setPokemonId(stoi(s));
		} else{
			file.get();
			p->setPokemonId(EMPTY);	
		}
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setStatID(stoi(s));
		} else{
			file.get();
			p->setStatID(EMPTY);	
		}
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setBaseStat(stoi(s));
		} else{
			file.get();
			p->setBaseStat(EMPTY);	
		}
		
		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setEffort(stoi(s));
		} else{
			file.get();
			p->setEffort(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}


std::vector<poke_class *> scanExperience(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/experience.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		experience *p = new experience();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setGrowthRateId(stoi(s));
		} else{
			file.get();
			p->setGrowthRateId(EMPTY);	
		}
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setLevel(stoi(s));
		} else{
			file.get();
			p->setLevel(EMPTY);	
		}
		
		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setExperience(stoi(s));
		} else{
			file.get();
			p->setExperience(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}

std::vector<poke_class *> scanTypeNames(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/type_names.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		type_name *p = new type_name();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setTypeId(stoi(s));
		} else{
			file.get();
			p->setTypeId(EMPTY);	
		}
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			//check for english lines only
			if(stoi(s) != 9){
				delete p;
				std::getline(file, s);
				continue;
			}
			p->setLocalLanguageID(stoi(s));
		} else{
			file.get();
			p->setLocalLanguageID(EMPTY);	
		}
		
		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setName(s);
		} else{
			file.get();
			p->setName(std::string());	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}

std::vector<poke_class *> scanPokemon(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/pokemon.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		pokemon_data *p = new pokemon_data();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setId(stoi(s));
		} else{
			file.get();
			p->setId(EMPTY);	
		}

		std::getline(file, s, ',');
		p->setIdentifier(s);
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setSpeciesID(stoi(s));
		} else{
			file.get();
			p->setSpeciesID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setHeight(stoi(s));
		} else{
			file.get();
			p->setHeight(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setWeight(stoi(s));
		} else{
			file.get();
			p->setWeight(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setBaseExperience(stoi(s));
		} else{
			file.get();
			p->setBaseExperience(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setOrder(stoi(s));
		} else{
			file.get();
			p->setOrder(EMPTY);	
		}

		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setIsDefault(stoi(s));
		} else{
			file.get();
			p->setIsDefault(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}

std::vector<poke_class *> scanPokeMoves(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/pokemon_moves.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		pokemon_move *p = new pokemon_move();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setPokeId(stoi(s));
		} else{
			file.get();
			p->setPokeId(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setVersionID(stoi(s));
		} else{
			file.get();
			p->setVersionID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setMoveID(stoi(s));
		} else{
			file.get();
			p->setMoveID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setMoveMethodID(stoi(s));
		} else{
			file.get();
			p->setMoveMethodID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setLevel(stoi(s));
		} else{
			file.get();
			p->setLevel(EMPTY);	
		}

		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setOrder(stoi(s));
		} else{
			file.get();
			p->setOrder(EMPTY);	
		}

		v.push_back(p);
	}

	file.close();
	return v;
}


std::vector<poke_class *> scanMoves(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/moves.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		move *p = new move();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setId(stoi(s));
		} else{
			file.get();
			p->setId(EMPTY);	
		}

		std::getline(file, s, ',');
		p->setIdentifier(s);
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setGenerationID(stoi(s));
		} else{
			file.get();
			p->setGenerationID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setTypeID(stoi(s));
		} else{
			file.get();
			p->setTypeID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setPower(stoi(s));
		} else{
			file.get();
			p->setPower(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setPP(stoi(s));
		} else{
			file.get();
			p->setPP(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setAccuracy(stoi(s));
		} else{
			file.get();
			p->setAccuracy(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setPriority(stoi(s));
		} else{
			file.get();
			p->setPriority(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setTargetID(stoi(s));
		} else{
			file.get();
			p->setTargetID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setDamageClassID(stoi(s));
		} else{
			file.get();
			p->setDamageClassID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setEffectID(stoi(s));
		} else{
			file.get();
			p->setEffectID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setEffectChance(stoi(s));
		} else{
			file.get();
			p->setEffectChance(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setContestTypeID(stoi(s));
		} else{
			file.get();
			p->setContestTypeID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setContestEffectID(stoi(s));
		} else{
			file.get();
			p->setContestEffectID(EMPTY);	
		}

		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setSuperContestEffectID(stoi(s));
		} else{
			file.get();
			p->setSuperContestEffectID(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}

std::vector<poke_class *> scanPokeSpecies(std::string path){
	std::vector<poke_class *> v;
	std::ifstream file;
	file.open(path + "/pokemon_species.csv");
	std::string s;

	//eat first line of metadata
	std::getline(file, s);
	
	//start reading pokemon
	while(file.peek() != EOF){
		pokemon_species *p = new pokemon_species();
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setId(stoi(s));
		} else{
			file.get();
			p->setId(EMPTY);	
		}

		std::getline(file, s, ',');
		p->setIdentifier(s);
		
		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setGenerationID(stoi(s));
		} else{
			file.get();
			p->setGenerationID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setEvolvesFromSpeciesID(stoi(s));
		} else{
			file.get();
			p->setEvolvesFromSpeciesID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setEvolutionChainID(stoi(s));
		} else{
			file.get();
			p->setEvolutionChainID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setColorID(stoi(s));
		} else{
			file.get();
			p->setColorID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setShapeID(stoi(s));
		} else{
			file.get();
			p->setShapeID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setHabitatID(stoi(s));
		} else{
			file.get();
			p->setHabitatID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setGenderRate(stoi(s));
		} else{
			file.get();
			p->setGenderRate(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setBaseHappiness(stoi(s));
		} else{
			file.get();
			p->setBaseHappiness(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setIsBaby(stoi(s));
		} else{
			file.get();
			p->setIsBaby(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setHatchCounter(stoi(s));
		} else{
			file.get();
			p->setHatchCounter(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setHasGenderDifferences(stoi(s));
		} else{
			file.get();
			p->setHasGenderDifferences(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setGrowthRateID(stoi(s));
		} else{
			file.get();
			p->setGrowthRateID(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setFormsSwitchable(stoi(s));
		} else{
			file.get();
			p->setFormsSwitchable(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setIsLegendary(stoi(s));
		} else{
			file.get();
			p->setIsLegendary(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setIsMythical(stoi(s));
		} else{
			file.get();
			p->setIsMythical(EMPTY);	
		}

		if(file.peek() != ','){
			std::getline(file, s, ',');
			p->setOrder(stoi(s));
		} else{
			file.get();
			p->setOrder(EMPTY);	
		}

		if(file.peek() != '\n'){
			std::getline(file, s);
			p->setConquestOrder(stoi(s));
		} else{
			file.get();
			p->setConquestOrder(EMPTY);	
		}
		v.push_back(p);
	}

	file.close();
	return v;
}


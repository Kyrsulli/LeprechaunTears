#pragma once

#include <vector>
#include <fstream>
#include "Tile.h"
#include "Tee.h"
#include "Cup.h"

class Level{
public:
	Level(int, std::string);
	~Level();
	void render();
	void addCup(Cup*);
	void addTee(Tee*);
	void addTiles(std::vector<Tile*>);
	float* getCupLocation();
	float* getTeeLocation();
private:
	std::ifstream levelData;
	std::vector<Tile*> tileList;
	std::string line;

	std::vector<Tile> tiles;
	int levelNumber;
	Tee* tee;
	Cup* hole;

	void errorExit(int);
	void get_input();

};



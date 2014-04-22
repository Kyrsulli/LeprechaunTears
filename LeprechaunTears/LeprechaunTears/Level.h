#pragma once

#include <vector>
#include "Tile.h"
#include "Tee.h"
#include "Cup.h"

class Level{
public:
	Level(int);
	~Level();
	void render();
	void addCup(Cup*);
	void addTee(Tee*);
private:
	std::vector<Tile> tiles;
	int levelNumber;
	Tee* tee;
	Cup* hole;
};



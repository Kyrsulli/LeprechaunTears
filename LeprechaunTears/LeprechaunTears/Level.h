#pragma once

#include <vector>
#include <fstream>
#include "Tile.h"
#include "Tee.h"
#include "Cup.h"
#include "PhysicsObject.h"

class Level{
public:
	Level(int, std::string);
	~Level();
	void update();
	void render();
	void addCup(Cup*);
	void addTee(Tee*);
	void addTiles(std::vector<Tile*>);
	float* getCupLocation();
	float* getTeeLocation();
	//float* getBallLocation();
	void addForce();
	void changeAngle(float f){physicsObjects[0]->changeAngle(f);};
	void changeMag(float f){physicsObjects[0]->changeMag(f);};
	bool levelComplete(){return completed;};
private:
	std::ifstream levelData;
	std::vector<Tile*> tileList;
	std::string line;

	std::vector<Tile> tiles;
	std::vector<PhysicsObject*> physicsObjects;
	PhysicsObject* ball;
	int levelNumber;
	Tee* tee;
	Cup* hole;
	bool completed;

	void errorExit(int);
	void get_input();
	double vec3Dist(glm::vec3, glm::vec3);

};



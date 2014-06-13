#pragma once

#include <vector>
#include <fstream>
#include "Tile.h"
#include "Tee.h"
#include "Cup.h"
#include "PhysicsObject.h"
#include "LTObject.h"

class Level : public LTObject{
public:
	Level(int, std::string);
	~Level();
	void update() override;
	void render() override;
	void addCup(Cup*);
	void addTee(Tee*);
	void addTiles(std::vector<Tile*>);
	float* getCupLocation();
	float* getTeeLocation();
	float* getBallLocation();
	void addForce();
	void changeAngle(float f){physicsObjects[0]->changeAngle(f);};
	void changeMag(float f){physicsObjects[0]->changeMag(f);};
	double vec3Dist(glm::vec3, glm::vec3);
	void setPar(int p){par = p;};
	int getPar(){return par;};
	void reset();
	void toggleDebug();
	std::string name;
private:
	std::ifstream levelData;
	std::vector<Tile*> tileList;
	std::string line;

	std::vector<Tile> tiles;
	std::vector<PhysicsObject*> physicsObjects;
	PhysicsObject* ball;

	int levelNumber;
	int par;
	Tee* tee;
	Cup* hole;

	bool debug;
};



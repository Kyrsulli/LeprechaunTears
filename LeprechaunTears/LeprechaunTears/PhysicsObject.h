#pragma once
#include <glm.hpp>
#include <vector>
#include "Tile.h"

class PhysicsObject{
public:
	PhysicsObject();
	~PhysicsObject();
	virtual void update(Tile*);
	virtual void render();
	void addForce(glm::vec3);
	virtual float getAngle(){return 0;};
	virtual float getMagnitude(){return 0;};
	virtual void changeAngle(float f){};
	virtual void changeMag(float f){};
	glm::vec3 getPosition(){return position;};
	virtual int getCurrentTile(std::vector<Tile>){return currentTile;}
protected:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;
	float friction;
	int currentTile;

	void addDrag();
};
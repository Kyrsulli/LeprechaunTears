#pragma once
#include <glm.hpp>
#include "Tile.h"

class PhysicsObject{
public:
	PhysicsObject();
	~PhysicsObject();
	virtual void update(Tile*);
	virtual void render();
	void addForce(glm::vec3);
	int getCurrentTile(){return currentTile;}
	glm::vec3 getPosition(){return position;}
protected:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;
	float friction;
	int currentTile;

	void addDrag();
};
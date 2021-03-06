#pragma once
#include "PhysicsObject.h"
#include <vector>
#include <glm.hpp>
#include "Tee.h"

class Ball : public PhysicsObject{
public:
	Ball(Tee*, int);
	~Ball();
	void update(Tile*) override;
	void render() override;
	float getAngle() override{return angle;};
	float getMagnitude() override{return magnitude/1000;};
	void changeAngle(float f) override{angle+=f;};
	void changeMag(float f) override{magnitude+=f;};
	int getCurrentTile(std::vector<Tile>) override;
private:
	float angle;
	float magnitude;
	bool bounce;
	glm::vec3 calculateBounceVector(glm::vec3);
	float Ball::calcDistanceToWall(Point* x1, Point* x2, glm::vec3 x3);
};
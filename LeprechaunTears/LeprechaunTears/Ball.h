#pragma once
#include "PhysicsObject.h"
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
private:
	float angle;
	float magnitude;
};
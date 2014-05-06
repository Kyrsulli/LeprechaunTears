#pragma once
#include "PhysicsObject.h"
#include "Tee.h"

class Ball : public PhysicsObject{
public:
	Ball(Tee*, int);
	~Ball();
	void update(Tile*) override;
	void render() override;
private:
	
};
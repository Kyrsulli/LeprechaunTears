#include <cstdlib>
#include <glut.h>
#include "Ball.h"
#include "Tee.h"
#include "glm.hpp"
#include "Point.h"

using namespace std;
using namespace glm;

Ball::Ball(Tee* t, int m){
	position.x = t->x;
	position.y = t->y;
	position.z = t->z;
	currentTile = t->id;
	velocity.x = velocity.y = velocity.z = 0;
	mass = m;
	friction = 0.99;
}

Ball::~Ball(){

}

void Ball::update(Tile* t){
	//update position
	position += velocity;
	addDrag();
	//update collisions
	vector<int> n = t->getNeigbhors();
	vector<Point*> v = t->getVertices();

}

void Ball::render() {
	glPushMatrix();{
		glColor3f(1, 1, 1);
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.085f, 30, 30);
	}glPopMatrix();
}
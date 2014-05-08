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
	angle = 180;
	magnitude = 50;
}

Ball::~Ball(){

}

void Ball::update(Tile* t){
	if(angle > 360)
		angle = 0;
	if(angle < 0)
		angle = 360;
	if(magnitude > 100)
		magnitude = 100;
	if(magnitude < 0)
		magnitude = 0;
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
		if(velocity.x == 0 && velocity.y == 0 && velocity.z == 0){
			glRotatef(angle, 0, 1, 0);
			glTranslatef(0, 0, 0.06*magnitude/20);
			glScalef(1, 1, magnitude/10);
			glutSolidCube(0.06);
		}
	}glPopMatrix();
}
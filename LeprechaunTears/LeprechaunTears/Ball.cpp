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
	bounce = false;
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
	/*if(t->getHeightAtPoint(position)!=0)
	printf("%f\n", t->getHeightAtPoint(position));*/
	position.y = t->getHeightAtPoint(position);
	//printf("%f",t->getHeightAtPoint(position));
	addDrag();
	//update collisions
	vector<int> n = t->getNeighbors();
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

int Ball::getCurrentTile(std::vector<Tile> tiles){
	if(tiles.empty())
		return currentTile;
	/*if(tiles[currentTile-1].withinBounds(position)==1)
		return currentTile;*/
	/*for(int j = 0; j < tiles.size(); j++){
		if(tiles[j].withinBounds(position)==1){
			if(tiles[j].getHeightAtPoint(position) >= tiles[currentTile-1].getHeightAtPoint(position)){
				currentTile = j+1;
				return j+1;
			}
			
		}
	}*/
	std::vector<int> neighbors = tiles[currentTile-1].getNeighbors();
	for(int i = 0; i < neighbors.size(); i++){
		if(neighbors[i]!=0){
			if(tiles[neighbors[i]-1].withinBounds(position) == 1){
				if(tiles[neighbors[i]-1].getHeightAtPoint(position)>=tiles[currentTile-1].getHeightAtPoint(position)){
					currentTile = neighbors[i];
					printf("%d\n", currentTile);
					return currentTile;
				}
			}
		}else{//neighbors[i] == 0
			//currentTile = neighbors[i];
		}
	}
	if(tiles[currentTile-1].withinBounds(position)==0 && !bounce){
		bounce = true;
		/*Velocity Calculations go here!*/
		//This is just temp to show that the collision works. We need to just make it bounce at the correct angles.
		//velocity = -velocity;
		//velocity = calculateBounceVector(tiles[currentTile-1].getWallNormal(currentTile-1));
		velocity = calculateBounceVector(glm::vec3(1, 0, 0));
	}
	else bounce = false;
	return currentTile;
}

vec3 Ball::calculateBounceVector(vec3 wallNormal){
	//http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	//r = d - 2(d(dot)n)n
	//r is reflection, d is direction, n is normal
	//d = velocity, n = wallNormal, r is return value
	glm::normalize(wallNormal);
	return velocity - 2 * glm::dot(velocity, wallNormal) * wallNormal;
	//return vec3(0, 0, 0);
}
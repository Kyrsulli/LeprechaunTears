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
	position.y = t->getHeightAtPoint(position);
		if(glm::normalize(t->faceNormal()) != glm::vec3(0, 1, 0)){//if it is on a slanted slope
			
			glm::vec3 foo = glm::cross(glm::vec3(0, 1, 0), t->faceNormal());
			glm::vec3 rollDir = glm::cross(glm::vec3(0, 1, 0), foo);
			position.y = t->getHeightAtPoint(position);
			
			addDrag(rollDir);
		}
		else addDrag(glm::vec3(0,0,0));
}

void Ball::render() {
	glPushMatrix();{
		glColor3f(1, 1, 1);
		glTranslatef(position.x, position.y + 0.05f, position.z);
		glutSolidSphere(0.04f, 30, 30);
		if(velocity.x == 0 && velocity.y == 0 && velocity.z == 0){
			glRotatef(angle, 0, 1, 0);
			glTranslatef(0, 0, 0.06*magnitude/20);
			glScalef(1, 1, magnitude/10);
			glutSolidCube(0.06);
		}
	}glPopMatrix();
}

int Ball::getCurrentTile(std::vector<Tile> tiles){
	//So that this doesn't crash before the level loads.
	if(tiles.empty())
		return currentTile;
	float distToDest = glm::length(velocity);
	std::vector<int> neighbors = tiles[currentTile-1].getNeighbors();
	int check=0;
	int edgePointIndex = -99999999;
			vector<Point*> v = tiles[currentTile-1].getVertices();
			//Check all the edges to see if the ball will pass over it.
			for(int j = 0; j < v.size(); j++){
				//Calculate for both where it will be and where it is, just to be safe.
				float distToWall = calcDistanceToWall(v[j], v[( j + 1 == v.size()?0:j + 1)], position);
				float distToWallSoon = calcDistanceToWall(v[j], v[( j + 1 == v.size()?0:j + 1)], position+velocity);
				if(distToWall <= distToDest || distToWallSoon <= distToDest){
					edgePointIndex = j;
					//Check to see what the tile is, or if it is a wall.
					if(neighbors[edgePointIndex] != 0 && tiles[neighbors[edgePointIndex]-1].withinBounds(position+velocity)){
						currentTile = neighbors[edgePointIndex];
						return currentTile;
					}
					//If it's a wall, we'll do some collision stuff.
					else if(neighbors[edgePointIndex] == 0)
						check=1;
					break;
				}
	}
	if(check==1&&tiles[currentTile-1].getNeighbors()[edgePointIndex] == 0){
		bounce = true;
		/*Velocity Calculations go here!*/
		velocity = calculateBounceVector(tiles[currentTile-1].getWallNormal(edgePointIndex));
	}
	else bounce = false;
	return currentTile;
}

inline vec3 Ball::calculateBounceVector(vec3 wallNormal){
	//http://math.stackexchange.com/questions/13261/how-to-get-a-reflection-vector
	//r = d - 2(d(dot)n)n
	//r is reflection, d is direction, n is normal
	//d = velocity, n = wallNormal, r is return value
	wallNormal = glm::normalize(wallNormal);
	return velocity - 2 * glm::dot(velocity, wallNormal) * wallNormal;
	//return vec3(0, 0, 0);
}

inline float Ball::calcDistanceToWall(Point* x0, Point* x1, glm::vec3 x2v){
	/*
	| (x0 - x1) [cross] (x0 - x2) | / | x2 - x1 |
	x0 is the ball, x1 and x2 are points of the edge
	http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	*/
	//make vectors of the first two points
	glm::vec3 x0v(x0->x, x0->y, x0->z);
	glm::vec3 x1v(x1->x, x1->y, x1->z);
	//return glm::length( glm::cross((x0v - x1v), (x0v - x2v)) ) / glm::length( x2v - x1v);
	return glm::length( glm::cross((x2v - x0v), (x2v - x1v)) ) / glm::length( x1v - x0v);
}
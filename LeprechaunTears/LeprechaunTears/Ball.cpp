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
	//if(glm::length(velocity) == 0){
		if(glm::normalize(t->faceNormal()) != glm::vec3(0, 1, 0)){//if it is on a slanted slope
			
			glm::vec3 foo = glm::cross(glm::vec3(0, 1, 0), t->faceNormal());
			glm::vec3 rollDir = glm::cross(glm::vec3(0, 1, 0), foo);
			printf("Roll Dir:%f %f %f\n", rollDir.x, rollDir.y, rollDir.z);
			printf("Velocity:%f %f %f\n", velocity.x, velocity.y, velocity.z);
			//printf("Sum     :%f %f %f\n\n", rollDir.x + position.x, rollDir.y + position.y, rollDir.z + position.z);
			//velocity.x -= rollDir.x/750;
			//velocity.z -= rollDir.z/750;
			position.y = t->getHeightAtPoint(position);
			
			
			//if(rollDir.y < 0){
			//	rollDir = -rollDir;
			//}
			//if(rollDir.y != 0){
				//velocity = rollDir;
			//	printf("%f %f %f\n", rollDir.x, rollDir.y, rollDir.z);
			//}
			addDrag(rollDir);
		}
		else addDrag(glm::vec3(0,0,0));
	//}
//}
}
	//update collisions
	//vector<int> n = t->getNeighbors();
	//vector<Point*> v = t->getVertices();

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
	int edgePointIndex = -99999999;
	for(int i = 0; i < neighbors.size(); i++){
		if(neighbors[i]!=0){
			if(tiles[neighbors[i]-1].withinBounds(position) == 1){
				if(tiles[neighbors[i]-1].getHeightAtPoint(position)>=tiles[currentTile-1].getHeightAtPoint(position) || tiles[currentTile-1].withinBounds(position)==0){
					currentTile = neighbors[i];
					printf("%d\n", currentTile);
					return currentTile;
				}
			}
		}else{//neighbors[i] == 0
			vector<Point*> v = tiles[currentTile-1].getVertices();
			for(int j = 0; j < v.size(); j++){
				float distToWall = calcDistanceToWall(v[j], v[( j + 1 == v.size()?0:j + 1)], position);
				if(distToWall <= 0.1){
					edgePointIndex = j;
					if(tiles[currentTile-1].getNeighbors()[edgePointIndex] != 0 && tiles[tiles[currentTile-1].getNeighbors()[edgePointIndex]-1].withinBounds(position)
						&&tiles[tiles[currentTile-1].getNeighbors()[edgePointIndex]-1].getHeightAtPoint(position)>=tiles[currentTile-1].getHeightAtPoint(position)){
						currentTile = tiles[currentTile-1].getNeighbors()[edgePointIndex];
						printf("%d\n", currentTile);
						return currentTile;
					}
					break;
				}
			}
		}
	}
	if(tiles[currentTile-1].withinBounds(position)==0 && !bounce && edgePointIndex!=-99999999 && tiles[currentTile-1].getNeighbors()[edgePointIndex] == 0){
		bounce = true;
		/*Velocity Calculations go here!*/
		//This is just temp to show that the collision works. We need to just make it bounce at the correct angles.
		/*
		if(edgePointIndex == -99999999){
			return currentTile;
		}*/
		//printf("%d\n", edgePointIndex);
		velocity = calculateBounceVector(tiles[currentTile-1].getWallNormal(edgePointIndex));
		//velocity = calculateBounceVector(glm::vec3(1, 0, 0));
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
	return glm::length( glm::cross((x0v - x1v), (x0v - x2v)) ) / glm::length( x2v - x1v);
}
#include <vector>
#include <glut.h>
#include <cstdlib>
#include <glm.hpp>
#include "Tile.h"

#define wallHeight 0.1
#define aLittleBit 0.005

Tile::Tile(int ID){
	id = ID;
	normalCalculated = false;
}

Tile::~Tile(){
	while(!vertices.empty()){
		vertices.pop_back();
	}
}

void Tile::addNeighbor(int val){
	neighbors.push_back(val);
}

void Tile::addVertex(float x, float y, float z){
	Point* newpoint = new Point(x, y, z);
	vertices.push_back(newpoint);
	//printf("vertex added\n");
}

void Tile::renderTile(){
	if(vertices.empty() || vertices.size() <= 2){ 
		printf("Not enough vertices have been added to tile %d\n", this->id); 
		exit(1);
	}
	if(!normalCalculated){
		calculateFaceNormal();
		calculateExtremes();
		normalCalculated = true;
	}
	glColor3f(0.0f, 0.8f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(normal.x, normal.y, normal.z);
	for(int i = 0; i < vertices.size(); i++){
		
		Point* p = vertices[i];
		glVertex3f(p->x, p->y, p->z);
	}
	glEnd();
	defineEdges();
	drawWalls();
}

inline void Tile::defineEdges(){
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for(int i = 0; i < vertices.size(); i++){
		
		Point* p = vertices[i];
		glVertex3f(p->x, p->y + aLittleBit, p->z);
	}
	glVertex3f(vertices[0]->x, vertices[0]->y + aLittleBit, vertices[0]->z);
	glEnd();
}

inline void Tile::drawWalls(){
	for(int n = 0; n < vertices.size(); n++){
		//check if we need to even draw a wall here
		if(neighbors[n] != 0){
			continue;
		}
		//get the 2 indexes to be edges
		Point* p1 = vertices[n];
		Point* p2 = vertices[(n + 1 >= vertices.size()?0:n+1)];
		Point* p3 = new Point(p1->x, p1->y + wallHeight, p1->z);
		glColor3f(0.75f, 0, 0);
		glm::vec3 wallNormal = calculateNormal(p1, p2, p3);
		glNormal3f(wallNormal.x, wallNormal.y, wallNormal.z);
		glBegin(GL_QUADS);
		glVertex3f(p1->x, p1->y, p1->z);
		glVertex3f(p1->x, p1->y + wallHeight, p1->z);
		glVertex3f(p2->x, p2->y + wallHeight, p2->z);
		glVertex3f(p2->x, p2->y, p2->z);
		glEnd();
	}
}

inline void Tile::calculateFaceNormal(){
	//this assumes that all points are in memory, and that there are at least 3
	Point* p0 = vertices[0];
	Point* p1 = vertices[1];
	Point* p2 = vertices[2];
	//grab the vector forms of the 3 points
	glm::vec3 v0(p0->x, p0->y, p0->z);
	glm::vec3 v1(p1->x, p1->y, p1->z);
	glm::vec3 v2(p2->x, p2->y, p2->z);
	//grab 2 vectors of the triangle
	glm::vec3 u = v2 - v0;
	glm::vec3 v = v2 - v1;

	float x = u.y * v.z - u.z * v.y;
	float y = u.z * v.x - u.x * v.z;
	float z = u.x * v.y - u.y * v.x;

	//delete the default normal made by the initialization of the class
	normal = glm::vec3(x, y, z);

/*
https://www.opengl.org/wiki/Calculating_a_Surface_Normal

Set Vector U to (Triangle.p2 minus Triangle.p1)
Set Vector V to (Triangle.p3 minus Triangle.p1)
 
Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
*/
	
}

glm::vec3 Tile::calculateNormal(Point* p0, Point* p1, Point* p2){
	glm::vec3 v0(p0->x, p0->y, p0->z);
	glm::vec3 v1(p1->x, p1->y, p1->z);
	glm::vec3 v2(p2->x, p2->y, p2->z);
	//grab 2 vectors of the triangle
	glm::vec3 u = v2 - v0;
	glm::vec3 v = v2 - v1;

	float x = u.y * v.z - u.z * v.y;
	float y = u.z * v.x - u.x * v.z;
	float z = u.x * v.y - u.y * v.x;

	return glm::vec3(x, y, z);
}

inline void Tile::calculateExtremes(){
	minx = vertices[0]->x;
	maxx = vertices[0]->x;
	miny = vertices[0]->y;
	maxy = vertices[0]->y;
	minz = vertices[0]->z;
	maxz = vertices[0]->z;
	for(int i = 1; i < vertices.size(); i++){
		if(vertices[i]->x < minx){
			minx = vertices[i]->x;
		}
		if(vertices[i]->x > maxx){
			maxx = vertices[i]->x;
		}
		if(vertices[i]->y < miny){
			miny = vertices[i]->y;
		}
		if(vertices[i]->y > maxy){
			maxy = vertices[i]->y;
		}
		if(vertices[i]->z < minz){
			minz = vertices[i]->z;
		}
		if(vertices[i]->z > maxz){
			maxz = vertices[i]->z;
		}
	}
}

float Tile::getHeightAtPoint(glm::vec3 position){
	if(!normalCalculated) return 0;
	if(withinBounds(position) == 0) return 0;
	//use bilinear interpolation to extract z values
	//glm::vec3 n = calculateNormal(new Point(minx,miny,minz), new Point(maxx,maxy,maxz),new Point(position.x,position.y,position.z));
	if(normal.y == 0) normal.y = 1;//printf("%f, %f, %f\n", normal.x, normal.y, normal.z);
	float y = (normal.x*(position.x - vertices[0]->x) - normal.y * vertices[0]->y + normal.z*(position.z - vertices[0]->z))/(-normal.y);
	//float t = (position.x - minx)/paramvec.x;
	return y;

	//return 0;
}

float Tile::withinBounds(glm::vec3 position){
	
	if(position.x > maxx || position.x < minx||
	   //position.y > maxy || position.y < miny  ||
	   position.z > maxz || position.z < minz){
		   //throw "Position is outside the extremes of this tile";
		   /*printf("Position is out of the extremes of this tile\n");
		   printf("X: %f, MaxX: %f, MinX: %f\n", position.x, maxx, minx);
		   printf("Z: %f, MaxZ: %f, MinZ: %f\n", position.z, maxz, minz);*/
		   return 0;
	}
	else return 1;
}

glm::vec3 Tile::getWallNormal(int n){
	Point* p1 = vertices[n];
	if(n+1 >= vertices.size())
		n = 0;
	else
		n = n + 1;
	//Point* p2 = vertices[(n + 1 == vertices.size()?0:n+1)];
	Point* p2 = vertices[n];
	Point* p3 = new Point(p1->x, p1->y + 1, p1->z);//create a point above one of the existing points.  Doesn't matter how tall
	glm::vec3 N = calculateNormal(p1, p2, p3);
	printf("Bounced off wall %d\n", neighbors[n]);
	//delete p3;
	return N;
}


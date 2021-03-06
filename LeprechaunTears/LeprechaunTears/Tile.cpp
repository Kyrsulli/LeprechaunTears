#include <vector>
#include <glut.h>
#include <cstdlib>
#include <glm.hpp>
#include "Tile.h"

#define wallHeight 0.1
#define aLittleBit 0.005

//constructor
Tile::Tile(int ID){
	id = ID;
	normalCalculated = false;
	debugColor.x = (float) rand() / RAND_MAX;
	debugColor.y = (float) rand() / RAND_MAX;
	debugColor.z = (float) rand() / RAND_MAX;
}

//destructor
Tile::~Tile(){
	while(!vertices.empty()){
		vertices.pop_back();
	}
}

//add a new neighbor, matches the points at index i and i + 1
void Tile::addNeighbor(int val){
	neighbors.push_back(val);
}

//add a new point.  Automatically connects last point to first
void Tile::addVertex(float x, float y, float z){
	Point* newpoint = new Point(x, y, z);
	vertices.push_back(newpoint);
	calculateExtremes();
}

//render the tile.  first check to make sure enough information is there for it to render, then run it
void Tile::render(bool debug){
	if(vertices.empty() || vertices.size() <= 2){ 
		printf("Not enough vertices have been added to tile %d\n", this->id); 
		exit(1);
	}
	//if it reaches the render step it shouldn't have any more vertices added, so calculate and store the normal for lighting purposes
	if(!normalCalculated){
		calculateFaceNormal();
		normalCalculated = true;
	}
	//decide on the color
	if(debug){
		glColor3f(debugColor.x, debugColor.y, debugColor.z);
	}else{
		glColor3f(0.0f, 0.8f, 0.0f);
	}
	//draw the base tile
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(normal.x, normal.y, normal.z);
	for(int i = 0; i < vertices.size(); i++){
		Point* p = vertices[i];
		glVertex3f(p->x, p->y, p->z);
	}
	glEnd();
	//define edges to make borders easier to see?
	if(debug){
		defineEdges();
	}
	//render the walls
	drawWalls(debug);
}

//traces over the points again, same used in the triangle fan above, and draw a line connecting them, just a hair above the points so they stand out
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

//trace over the same points used in drawing the tile edge, draw walls
inline void Tile::drawWalls(bool debug){
	for(int n = 0; n < vertices.size(); n++){
		//check if we need to even draw a wall here
		if(neighbors[n] != 0){
			continue;
		}
		//get the 2 indexes to be edges
		Point* p1 = vertices[n];
		//the ?: handles the wrap around from the last to the first
		Point* p2 = vertices[(n + 1 >= vertices.size()?0:n+1)];
		//pick a third point above one of the points for wall normal info, since you need 3 points
		Point* p3 = new Point(p1->x, p1->y + wallHeight, p1->z);
		//calculate lighting!
		glm::vec3 wallNormal = calculateNormal(p1, p2, p3);
		//draw normals if debug option is on
		if(debug){
			glColor3f(0, 0, 0);
			glLineWidth(3);
			glBegin(GL_LINES);
			glVertex3f(p3->x, p3->y, p3->z);
			glVertex3f(p3->x + wallNormal.x, p3->y + wallNormal.y, p3->z + wallNormal.z);
			glEnd();
			glLineWidth(1);
		}
		//set the normal, color, and draw
		glNormal3f(wallNormal.x, wallNormal.y, wallNormal.z);
		if(debug){
			glColor3f(debugColor.x, debugColor.y, debugColor.z);
		}else{
			glColor3f(0.75f, 0, 0);
		}
		glBegin(GL_QUADS);
		glVertex3f(p1->x, p1->y, p1->z);
		glVertex3f(p1->x, p1->y + wallHeight, p1->z);
		glVertex3f(p2->x, p2->y + wallHeight, p2->z);
		glVertex3f(p2->x, p2->y, p2->z);
		glEnd();
	}
}

//calculate the normal for lighting purposes
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

//generalized version of the above, easier to handle walls his way
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

//calculate the extremities of x, y, and z.  Helps with physics
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

//calculates the height of the tile at a point, useful in rendering the ball for 2D physics cheat
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

//checks if the ball is in the tile bounds
float Tile::withinBounds(glm::vec3 position){
	
	if(position.x > maxx || position.x < minx||
	   position.z > maxz || position.z < minz){
		   return 0;
	}
	else return 1;
}

//calculate the normal of the wall for bouncing.  takes in the index of the point that defines the start of this wall
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
	return N;
}

//retrieve the extremities of the tile in the form of an array
// {minx, maxx, miny, maxy, minz, maxz}
float* Tile::getExtremes(){
	float* returnVal = new float[6];
	returnVal[0] = minx;
	returnVal[1] = maxx;
	returnVal[2] = miny;
	returnVal[3] = maxy;
	returnVal[4] = minz;
	returnVal[5] = maxz;
	return returnVal;
}

#include <vector>
#include <glut.h>
#include <cstdlib>
#include "Tile.h"

#define wallHeight 0.5
#define aLittleBit 0.005

Tile::Tile(int ID){
	id = ID;
}

Tile::~Tile(){

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
	if(vertices.empty()){ 
		printf("Vertices in tile ID %d is empty\n", this->id); 
		exit(1);
	}
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < vertices.size(); i++){
		
		Point* p = vertices[i];
		glVertex3f(p->x, p->y, p->z);
	}
	glEnd();
	defineEdges();
	drawWalls();
}

void Tile::defineEdges(){
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for(int i = 0; i < vertices.size(); i++){
		
		Point* p = vertices[i];
		glVertex3f(p->x, p->y + aLittleBit, p->z);
	}
	glVertex3f(vertices[0]->x, vertices[0]->y + aLittleBit, vertices[0]->z);
	glEnd();
}

void Tile::drawWalls(){
	for(int n = 0; n < vertices.size(); n++){
		//check if we need to even draw a wall here
		if(neighbors[n] != 0){
			continue;
		}
		//get the 2 indexes to be edges
		Point* p1 = vertices[n];
		Point* p2 = vertices[(n + 1 >= vertices.size()?0:n+1)];
		glColor3f(1.0f, 0, 0);
		glBegin(GL_QUADS);
		glVertex3f(p1->x, p1->y, p1->z);
		glVertex3f(p1->x, p1->y + wallHeight, p1->z);
		glVertex3f(p2->x, p2->y + wallHeight, p2->z);
		glVertex3f(p2->x, p2->y, p2->z);
		glEnd();
	}
}

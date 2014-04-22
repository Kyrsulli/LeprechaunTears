#include <vector>
#include <glut.h>
#include <cstdlib>
#include "Tile.h"

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
}
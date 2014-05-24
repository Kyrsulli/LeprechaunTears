#pragma once
#include <vector>
#include <glm.hpp>
#include "util.h"
#include "Point.h"

class Tile{
public:
	Tile(int);
	~Tile();

	void addNeighbor(int val);
	void addVertex(float x, float y, float z);
	void render();
	float getHeightAtPoint(glm::vec3);
	float withinBounds(glm::vec3);
	int getID(){return id;}
	std::vector<int> getNeighbors(){ return neighbors;}
	std::vector<Point*> getVertices(){ return vertices;}
	glm::vec3 getWallNormal(int);//returns the normal of the wall defined by points n, and n + 1
	glm::vec3 faceNormal(){return normal;};

private:
	int id;
	bool normalCalculated;
	std::vector<int> neighbors;
	std::vector<Point*> vertices;
	glm::vec3 normal;

	float minx, miny, minz, maxx, maxy, maxz;

	void defineEdges();
	void drawWalls();
	void calculateFaceNormal();
	glm::vec3 calculateNormal(Point*, Point*, Point*);
	void calculateExtremes();
};


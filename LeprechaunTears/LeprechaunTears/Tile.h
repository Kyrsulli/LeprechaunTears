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
	void renderTile();
	float getHeightAtPoint(glm::vec3);

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


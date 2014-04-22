#pragma once
#include <vector>
#include "util.h"
#include "Point.h"

class Tile{
public:
	Tile(int);
	~Tile();

	void addNeighbor(int val);
	void addVertex(float x, float y, float z);
	void renderTile();

private:
	int id;
	std::vector<int> neighbors;
	std::vector<Point*> vertices;
	Vector3* normal;

};


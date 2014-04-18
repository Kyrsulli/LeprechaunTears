#include "util.h"
#include "Level.h"
;
class Tile{
public:
	Tile(int, Level*);
	~Tile();

	void addEdge();
	void addNeighbor();
	void addVertex(int x, int y, int z);
	void addVertices(int** v);

private:
	int id;
	int* edges;
	int* neighbors;
	int** vertices;
	Level* level;
	Vector3* normal;

};


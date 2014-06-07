#include <vector>
#include <iostream>
#include <ctime>
#include "Level.h"
#include "LTObject.h"

#define MAX_TILE_SIDE_LENGTH 7

using namespace std;
using namespace glm;

template<typename T>
inline void constrain(T& curr, T min, T max){
	if(curr > max){
		curr = max;
		return;
	}
	if(curr < min){
		curr = min;
		return;
	}
	return;
}

inline string getRandomName(){
	cout << "Random name not yet implemented" << endl;
	return "";
}

inline Cup* getCup(vector<Tile*> tiles){
	Tile* lastTile = tiles[tiles.size() - 1];
	float* tileBounds = lastTile->getExtremes();
	//find the middle of it
	float x = (tileBounds[0] + tileBounds[1] ) / 2;
	float y = (tileBounds[2] + tileBounds[3] ) / 2;
	float z = (tileBounds[4] + tileBounds[5] ) / 2;
	//cleanup
	delete [] tileBounds;
	//return a new tee in the middle
	return new Cup(lastTile->getID(), x, y, z);
}

inline Tee* getTee(vector<Tile*> tiles){
	Tile* firstTile = tiles[0];
	float* tileBounds = firstTile->getExtremes();
	//find the middle of it
	float x = (tileBounds[0] + tileBounds[1] ) / 2;
	float y = (tileBounds[2] + tileBounds[3] ) / 2;
	float z = (tileBounds[4] + tileBounds[5] ) / 2;
	//cleanup
	delete [] tileBounds;
	//return a new tee in the middle
	return new Tee(firstTile->getID(), x, y, z);
}

inline vector<Tile*> getTiles(int complexity){
	vector<Tile*> course;//add tiles into this one by 1
	//these define the two points of the current tile that will be used to connect to the next tile
	vec3 p1;
	vec3 p2;
	srand(time(NULL));
	Tile* t = nullptr;
	for(int i = 1; i <= complexity; i++){//i being used for tile ID's also, so it can't be 0 because an ID of 0 defines a null neighbor, hence a wall
		//make new tile
		t = new Tile(i);
		//generate tile corners
		float dx = rand() % MAX_TILE_SIDE_LENGTH;
		float dz = rand() % MAX_TILE_SIDE_LENGTH;
		t->addVertex(p1.x, p1.y, p1.z);
		t->addVertex(p1.x + dx, p1.y, p1.z);
		t->addVertex(p1.x + dx, p1.y, p1.z + dz);
		t->addVertex(p1.x, p1.y, p1.z + dz);
		//figure out connection to last neighbor
		if(p1.x != 0 && p1.y != 0 && p1.z != 0){//not the first tile
			for(int i = 0; i < 4; i++){
				t->addNeighbor(0);
			}
		}
		//figure out connection to next neighbor

		//add to the list
		course.push_back(t);
	}
	return course;
}

inline Level* newLevel(int num, int complexity){
	//create the new level
	Level* newLevel = new Level(num, "");
	vector<Tile*> tiles = getTiles(complexity);
	newLevel->addTiles(tiles);
	newLevel->addCup(getCup(tiles));
	newLevel->addTee(getTee(tiles));
	newLevel->name = getRandomName();
	int c = complexity - 1;
	constrain<int>(c, 2, 10);
	newLevel->setPar(c);
	return newLevel;
}

vector<LTObject*> proceduralCourse(int holes){
	vector<LTObject*> course;
	for(int i = 0; i < holes; i++){
		course.push_back(newLevel(i, i/2 + 2));
	}
	return course;
}
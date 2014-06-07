#include <vector>
#include <iostream>
#include "Level.h"
#include "LTObject.h"

using namespace std;
using namespace glm;

template<typename T>
void constrain(T& curr, T min, T max){
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

string getRandomName(){
	cout << "Random name not yet implemented" << endl;
	return "";
}

Cup* getCup(vector<Tile*> tiles){
	Tile* lastTile = tiles[tiles.size() - 1];
	float* tileBounds = lastTile->getExtremes();
	//find the middle of it
	float x = (tileBounds[0] + tileBounds[1] ) / 2;
	float y = (tileBounds[2] + tileBounds[3] ) / 2;
	float z = (tileBounds[4] + tileBounds[5] ) / 2;
	//cleanup
	delete [] tileBounds;
	//return a new tee in the middle
	return new Cup(0, x, y, z);
}

Tee* getTee(vector<Tile*> tiles){
	Tile* firstTile = tiles[0];
	float* tileBounds = firstTile->getExtremes();
	//find the middle of it
	float x = (tileBounds[0] + tileBounds[1] ) / 2;
	float y = (tileBounds[2] + tileBounds[3] ) / 2;
	float z = (tileBounds[4] + tileBounds[5] ) / 2;
	//cleanup
	delete [] tileBounds;
	//return a new tee in the middle
	return new Tee(0, x, y, z);
}

vector<Tile*> getTiles(int complexity){
	vector<Tile*> course;//add tiles into this one by 1
	//these define the two points of the current tile that will be used to connect to the next tile
	vec3 p1;
	vec3 p2;
	for(int i = 1; i <= complexity; i++){//i being used for tile ID's also, so it can't be 0 because an ID of 0 defines a null neighbor, hence a wall
		//generate tile corners
		//figure out connection to last neighbor
		//figure out connection to next neighbor
	}
	return course;
}

Level* newLevel(int num, int complexity){
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
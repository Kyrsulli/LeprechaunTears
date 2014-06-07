#include <vector>
#include "Level.h"
#include "LTObject.h"

using namespace std;

/*
	void setPar(int p){par = p;};
	void addCup(Cup*);
	void addTee(Tee*);
	void addTiles(std::vector<Tile*>);
*/

Cup* getCup(vector<Tile*> tiles){
	return nullptr;
}

Tee* getTee(vector<Tile*> tiles){
	return nullptr;
}

Tile* getTiles(int complexity){
	return nullptr;
}

Level* newLevel(int num, int complexity){
	//create the new level
	Level* newLevel = new Level(num, "");
	vector<Tile*> tiles;
	for(int i = 0; i < complexity; i++){
		tiles.push_back(getTiles(complexity));
	}
	newLevel->addTiles(tiles);
	newLevel->addCup(getCup(tiles));
	newLevel->addTee(getTee(tiles));
	newLevel->setPar(complexity - 1);
	return newLevel;
}

vector<LTObject*> proceduralCourse(int holes){
	vector<LTObject*> course;
	for(int i = 0; i < holes; i++){
		course.push_back(newLevel(i, i/2 + 2));
	}
	return course;
}
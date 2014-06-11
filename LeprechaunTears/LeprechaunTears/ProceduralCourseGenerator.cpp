#include <vector>
#include <iostream>
#include <ctime>
#include <string>
#include "Level.h"
#include "LTObject.h"

#define MAX_TILE_SIDE_LENGTH 4

using namespace std;
using namespace glm;

const string word1[] = {"Unending", "Eternal", "Incomparable", "Spiralling", "Rocking", "Static", "Dynamic", "Wilting", "Metal"};
const string word2[] = {"Valley", "Mountain", "River", "Crucible", "Firepit", "Grassland", "Swamp", "Castle", "Metropolis"};
const string word3[] = {"Destruction", "Annihilation", "Fire", "Storms", "Brutality", "Perfection", "Glory", "Magic", "Light", "Darkness"};
const int word1len = 9;
const int word2len = 9;
const int word3len = 10;


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
	int a = rand() % word1len;
	int b = rand() % word2len;
	int c = rand() % word3len;
	return " The " + word1[a] + " " + word2[b] + " of " + word3[c];
}

inline Cup* getCup(vector<Tile*> tiles){
	Tile* lastTile = tiles[tiles.size() - 1];
	float* tileBounds = lastTile->getExtremes();
	//find the middle of it
	float x = (tileBounds[0] + tileBounds[1] ) / 2;
	float y = (tileBounds[2] + tileBounds[3] ) / 2;
	float z = (tileBounds[4] + tileBounds[5] ) / 2;
	//get a little variation
	float jiggleX = ((rand() % 3) - 1.5f) * 0.8f;
	float jiggleZ = ((rand() % 3) - 1.5f) * 0.8f;
	x += jiggleX;
	z += jiggleZ;
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
	//get a little variation
	float jiggleX = ((rand() % 3) - 1.5f) * 0.8f;
	float jiggleZ = ((rand() % 3) - 1.5f) * 0.8f;
	x += jiggleX;
	z += jiggleZ;
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
	Tile* t = nullptr;
	for(int i = 1; i <= complexity; i++){//i being used for tile ID's also, so it can't be 0 because an ID of 0 defines a null neighbor, hence a wall
		//make new tile
		t = new Tile(i);
		//generate tile corners
		if(i == 1){//first tile
			//width and height
			float w = (rand() % MAX_TILE_SIDE_LENGTH) + 1.5;
			float h = (rand() % MAX_TILE_SIDE_LENGTH) + 1.5;
			//add vertices of tile
			t->addVertex(0, 0, 0);
			t->addVertex(0, 0, h);
			t->addVertex(w, 0, h);
			t->addVertex(w, 0, 0);
			//add neighbors, hard code first neighbor
			t->addNeighbor(0);
			t->addNeighbor(0);
			t->addNeighbor(2);
			t->addNeighbor(0);
			//store the 2 points of connection so the next tile knows where to grab on to
			p1.x = w;
			p1.y = 0;
			p1.z = h;
			p2.x = w;
			p2.y = 0;
			p2.z = 0;
		}else if(i == complexity){//last tile
			//add first two points and their neighbor in
			t->addVertex(p2.x, p2.y, p2.z);
			t->addVertex(p1.x, p1.y, p1.z);
			t->addNeighbor(i - 1);
			//grab a new side length
			float s = (rand() % MAX_TILE_SIDE_LENGTH) + 1.5;
			//figure out if we are adding to the x or z axis
			p1.x += s;
			p2.x += s;
			t->addVertex(p1.x, p1.y, p1.z);
			t->addVertex(p2.x, p2.y, p2.z);
			//add the appropriate neighbors in
			for(int i = 0; i < 3; i++)
				t->addNeighbor(0);
		}else{//middle tiles
			//add first two points and their neighbor in
			t->addVertex(p2.x, p2.y, p2.z);
			t->addVertex(p1.x, p1.y, p1.z);
			t->addNeighbor(i - 1);
			//grab a new side length
			float s = (rand() % MAX_TILE_SIDE_LENGTH) + 1.5;
			//figure out if we are adding to the x or z axis
			int newDir = rand() % 3;//randomly choose the new direction
			switch(newDir){
			case 0://go left
				//move the 2 points over and put them in the tile
				p1.z += s;
				p2.z += s;
				t->addVertex(p1.x, p1.y, p1.z);
				t->addVertex(p2.x, p2.y, p2.z);
				//add the appropriate neighbors in
				t->addNeighbor(i + 1);
				t->addNeighbor(0);
				t->addNeighbor(0);
				//pick 2 new points
				p2 = p1;
				p1.z -= s;
				break;
			case 1://go straight up
				//move the 2 points over and put them in the tile
				p1.x += s;
				p2.x += s;
				t->addVertex(p1.x, p1.y, p1.z);
				t->addVertex(p2.x, p2.y, p2.z);
				//add the appropriate neighbors in
				t->addNeighbor(0);
				t->addNeighbor(i + 1);
				t->addNeighbor(0);
				break;
			case 2://go right
				//move the 2 points over and put them in the tile
				p1.z -= s;
				p2.z -= s;
				t->addVertex(p1.x, p1.y, p1.z);
				t->addVertex(p2.x, p2.y, p2.z);
				//add the appropriate neighbors in
				t->addNeighbor(0);
				t->addNeighbor(0);
				t->addNeighbor(i + 1);
				//pick 2 new points
				p2 = p1;
				p1.z += s;
				break;
			}
		}
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
	//seed the random numbers
	srand(time(NULL));
	//create the course
	vector<LTObject*> course;
	//create each level
	for(int i = 0; i < holes; i++){
		course.push_back(newLevel(i, i/2 + 2));
	}
	return course;
}
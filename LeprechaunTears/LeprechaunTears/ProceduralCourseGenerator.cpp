#include <vector>
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include "Level.h"
#include "LTObject.h"

#define SCALEFACTOR	0.25f

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

//next 3 functions from:
//http://www.dreamincode.net/forums/topic/66480-perlin-noise/
inline double findnoise2(double x,double y){
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x){
	double ft=x * 3.1415927;
	double f=(1.0-cos(ft))* 0.5;
	return a*(1.0-f)+b*f;
}

double noise(double x,double y){
	double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
	double floory=(double)((int)y);
	double s,t,u,v;//Integer declaration
	s=findnoise2(floorx,floory);
	t=findnoise2(floorx+1,floory);
	u=findnoise2(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
	v=findnoise2(floorx+1,floory+1);
	double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
	double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
	return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

float scaleNoise(int i, int j){
	double result = noise(i, j);
	result *= SCALEFACTOR;//scale it
	return static_cast<float>(result);
}

inline int convert(int i, int j, int a){
	return a * i + j + 1;
}

inline vector<Tile*> getTiles(int a, int b){
	vector<Tile*> course;//add tiles into this one by 1
	Tile* t = nullptr;
	const int yPerlinConstant = 1;
	for(int i = 0; i < a; i++){//row
		for(int j = 0; j < b; j++){//column
			t = new Tile(convert(i, j, a));
			//redefine specific tiles
//			if(convert(i, j, a) == 1){//first tile
//				t->addVertex(i    , 0, j);
//				t->addVertex(i    , 0, j + 1);
//				t->addVertex(i + 1, 0, j + 1);
//				t->addVertex(i + 1, 0, j);
//			}else{//other tiles
				//all of the ?:s are there to check for the first tile override
				t->addVertex(i    , (i     < 2 ? 0 : scaleNoise( i    , yPerlinConstant)), j);
				t->addVertex(i    , (i     < 2 ? 0 : scaleNoise( i    , yPerlinConstant)), j + 1);
				t->addVertex(i + 1, (i + 1 < 2 ? 0 : scaleNoise( i + 1, yPerlinConstant)), j + 1);
				t->addVertex(i + 1, (i + 1 < 2 ? 0 : scaleNoise( i + 1, yPerlinConstant)), j);
//			}
			//neighbor 1
			if(i == 0)
				t->addNeighbor(0);
			else
				t->addNeighbor(convert(i - 1, j, a));
			//neighbor 2
			if(j + 1 < b)
				t->addNeighbor(convert(i, j + 1, a));
			else
				t->addNeighbor(0);
			//neighbor 3
			if(i + 1 < a)
				t->addNeighbor(convert(i + 1, j, a));
			else
				t->addNeighbor(0);
			//neighbor 4
			if(j == 0)
				t->addNeighbor(0);
			else
				t->addNeighbor(convert(i, j - 1, a));
			//add it to the level
			course.push_back(t);
		}
	}
	return course;
}

inline Level* newLevel(int num, int complexity){
	//create the new level
	Level* newLevel = new Level(num, "");
	vector<Tile*> tiles = getTiles(complexity, 1/*std::max(complexity / 3, 1)*/);
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
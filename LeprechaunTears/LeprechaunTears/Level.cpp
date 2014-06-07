#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Tile.h"
#include "Level.h"
#include "Ball.h"
#include "util.h"

#define WINTHRESHOLD 0.1
#define VELOCITYTHRESHOLD 0.05

using namespace std;

Level::Level(int number, std::string file){
	levelNumber = number;
	tee = nullptr;
	hole = nullptr;
	ball = nullptr;
	completed = false;
	debug = false;
	//levelData.open(file);
	//get_input();
	//levelData.close();
}

Level::~Level(){

}

void Level::update(){
	for(int i = 0; i < physicsObjects.size(); i++){
		int currentTileID = physicsObjects[i]->getCurrentTile(tiles);
		Tile* currentTile;
		for(int j = 0; j < tiles.size(); j++){
			if(tiles[j].getID() == currentTileID){
				currentTile = &tiles[j];
				break;
			}
		}
//		printf("%d, %d\n", currentTile->getID(), currentTileID);
		physicsObjects[i]->update(currentTile);
		//check win condition
		if(physicsObjects[0]->getCurrentTile(tiles) == hole->id){
			glm::vec3 ballPos = physicsObjects[0]->getPosition();
			glm::vec3 cupPos(hole->x, hole->y, hole->z);
			if(vec3Dist(ballPos, cupPos) <=	WINTHRESHOLD){//if the ball is close enough to the cup
				//is the ball going too fast?
				if(glm::length(ball->getVelocity()) < VELOCITYTHRESHOLD){
					//wprintf("%f\n", glm::length(ball->getVelocity()));
					completed = true;
				}else{
					//if it is going too fast, knock it around a bit
					//get random floats to add to x and z
					//float rx = (static_cast<float>(rand() % 600) - 300.0f) / 1000.0f;
					//float rz = (static_cast<float>(rand() % 600) - 300.0f) / 1000.0f;
					physicsObjects[0]->changeAngle(rand() % 360);
					addForce();
					//printf("random values: %f %f\n", rx, rz);
					//ball->addForce(glm::vec3(rx, 0, rz));
				}
			}
		}
	}
}

void Level::render(){
	if(tiles.size() == 0){
		std::cout << "No tiles have been added to level " << levelNumber << ". Press Enter to exit." << std::flush;
		std::cin.ignore( std::numeric_limits <std::streamsize>::max(), '\n');
		exit(1);
	}
	if(hole == nullptr){
		std::cout << "No cup has been added to level " << levelNumber << ". Press Enter to exit." << std::flush;
		std::cin.ignore( std::numeric_limits <std::streamsize>::max(), '\n');
		exit(1);
	}
	if(tee == nullptr){
		std::cout << "No tee has been added to level " << levelNumber << ". Press Enter to exit." << std::flush;
		std::cin.ignore( std::numeric_limits <std::streamsize>::max(), '\n');
		exit(1);
	}
	
	for(Tile &t: tiles){
		t.render(debug);
	}
	hole->renderCup();
	tee->renderTee();
	for(int i = 0; i < physicsObjects.size(); i++){
		physicsObjects[i]->render();
	}
	
}

void Level::addTiles(std::vector<Tile*> t){
	for(int i = 0; i < t.size(); i++){
		tiles.push_back(*t[i]);
	}
}

void Level::addCup(Cup* c){
	hole = c;
}

void Level::addTee(Tee* t){
	tee = t;
	Ball* b = new Ball(tee, 1);
	physicsObjects.push_back(b);
	ball = b;
}

float* Level::getCupLocation(){
	float* pos = new float[3];
	pos[0] = hole->x;
	pos[1] = hole->y;
	pos[2] = hole->z;
	return pos;
}

float* Level::getTeeLocation(){
	float* pos = new float[3];
	pos[0] = tee->x;
	pos[1] = tee->y;
	pos[2] = tee->z;
	return pos;
}

float* Level::getBallLocation(){
	float* pos = new float[3];
	glm::vec3 pos2 = ball->getPosition();
	pos[0] = pos2.x;
	pos[1] = pos2.y;
	pos[2] = pos2.z;
	return pos;
}
/*
void Level::errorExit(int i){
	//printf("Error on line %d. Press any key followed by Enter to exit.", i);
	cout << "Error on line " << i << ". Press Enter to exit." << flush;
	cin.ignore( numeric_limits <streamsize>::max(), '\n');
	exit(1);
}

void Level::get_input(){
	int linecount = 0;
	while (!levelData.eof())
	{
		std::getline(levelData, line);
		std::istringstream iss(line);
		std::string type;
		int type_int = 0;
		if (!(iss >> type)) { break; } // error
		if(type.compare("tile") == 0) type_int = 0;
		if(type.compare("cup") == 0) type_int = 1;
		if(type.compare("tee") == 0) type_int = 2;
		Tile* newTile;
		Cup* newCup;
		Tee* newTee;
		int tileIndex;
		int edgeCount;
		float tempx, tempy, tempz;

		switch(type_int){

			case(0):
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			if(!(iss >> edgeCount)) { errorExit(linecount); }
			newTile = new Tile(tileIndex);
			for(int i = 0; i < edgeCount; i++){
				if(!(iss >> tempx)) { errorExit(linecount); }
				if(!(iss >> tempy)) { errorExit(linecount); }
				if(!(iss >> tempz)) { errorExit(linecount); }
				newTile->addVertex(tempx, tempy, tempz);
			}
		
			int tempEdge;
			for(int i = 0; i < edgeCount; i++){
				if(!(iss >> tempEdge)) { errorExit(linecount); }
				newTile->addNeighbor(tempEdge);
	
			}
			tileList.push_back(newTile);
			break;

			case(1):
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			if(!(iss >> tempx)) { errorExit(linecount); }
			if(!(iss >> tempy)) { errorExit(linecount); }
			if(!(iss >> tempz)) { errorExit(linecount); }
			newCup = new Cup(tileIndex, tempx, tempy, tempz);
			//level->addCup(newCup);
/*			break;
	
			case(2):
			int tileIndex;
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			float tempx, tempy, tempz;
			if(!(iss >> tempx)) { errorExit(linecount); }
			if(!(iss >> tempy)) { errorExit(linecount); }
			if(!(iss >> tempz)) { errorExit(linecount); }
			newTee = new Tee(tileIndex, tempx, tempy, tempz);
			//level->addTee(newTee);
/*			break;
	
			default:
			errorExit(linecount);
			break;
		}
		linecount++;

 // process pair (a,b)
	}
	addTiles(tileList);
	//put list of tiles into level
}
*/
void Level::addForce(){
	const float pi = 3.1415926535897;
	//printf("%f, %f\n", physicsObjects[0]->getAngle(), physicsObjects[0]->getAngle()*pi/180);
	physicsObjects[0]->addForce(glm::vec3(sin(physicsObjects[0]->getAngle()*pi/180) * physicsObjects[0]->getMagnitude(), 0, cos(physicsObjects[0]->getAngle()*pi/180) * physicsObjects[0]->getMagnitude()));
}


inline double Level::vec3Dist(glm::vec3 a, glm::vec3 b){
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;

	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void Level::reset(){
	Ball* b = new Ball(tee, 1);
	delete physicsObjects[0];
	physicsObjects[0] = b;
	ball = b;
}

void Level::toggleDebug(){
	debug = !debug;
}
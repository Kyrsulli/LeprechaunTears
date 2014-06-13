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

//create this level object
Level::Level(int number, std::string file){
	levelNumber = number;
	tee = nullptr;
	hole = nullptr;
	ball = nullptr;
	completed = false;
	debug = false;
}

//nothing to destruct that isn't handled by the compiler
Level::~Level(){

}

//update all physics objects, get their tiles
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
		physicsObjects[i]->update(currentTile);
		//check win condition
		if(physicsObjects[0]->getCurrentTile(tiles) == hole->id){
			glm::vec3 ballPos = physicsObjects[0]->getPosition();
			glm::vec3 cupPos(hole->x, hole->y, hole->z);
			if(vec3Dist(ballPos, cupPos) <=	WINTHRESHOLD){//if the ball is close enough to the cup
				//is the ball going too fast?
				if(glm::length(ball->getVelocity()) < VELOCITYTHRESHOLD){
					completed = true;
				}else{
					//if it is going too fast, knock it around a bit
					physicsObjects[0]->changeAngle(rand() % 360);
					addForce();
				}
			}
		}
	}
}

//render this level.  First make sure there are enough tiles, a hole, and a tee, then render them all along with the physics objects
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

//take in a list of tiles and store them
void Level::addTiles(std::vector<Tile*> t){
	for(int i = 0; i < t.size(); i++){
		tiles.push_back(*t[i]);
	}
}

//add a cup to this level
void Level::addCup(Cup* c){
	hole = c;
}

//add a tee to this level.  This implies the ball location, so put it there as well
//assign the pointer ball to b as a shortcut into the physics object list
void Level::addTee(Tee* t){
	tee = t;
	Ball* b = new Ball(tee, 1);
	physicsObjects.push_back(b);
	ball = b;
}

//return the locatoin of the cup
float* Level::getCupLocation(){
	float* pos = new float[3];
	pos[0] = hole->x;
	pos[1] = hole->y;
	pos[2] = hole->z;
	return pos;
}

//return the location of the tee
float* Level::getTeeLocation(){
	float* pos = new float[3];
	pos[0] = tee->x;
	pos[1] = tee->y;
	pos[2] = tee->z;
	return pos;
}

//return the location of the ball
float* Level::getBallLocation(){
	float* pos = new float[3];
	glm::vec3 pos2 = ball->getPosition();
	pos[0] = pos2.x;
	pos[1] = pos2.y;
	pos[2] = pos2.z;
	return pos;
}

//add force to the ball
void Level::addForce(){
	const float pi = 3.1415926535897;
	//printf("%f, %f\n", physicsObjects[0]->getAngle(), physicsObjects[0]->getAngle()*pi/180);
	physicsObjects[0]->addForce(glm::vec3(sin(physicsObjects[0]->getAngle()*pi/180) * physicsObjects[0]->getMagnitude(), 0, cos(physicsObjects[0]->getAngle()*pi/180) * physicsObjects[0]->getMagnitude()));
}

//calculate the distance between the points defined by 2 vec3's
inline double Level::vec3Dist(glm::vec3 a, glm::vec3 b){
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;

	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

//reset the level, since the engine preserves the level state after transitions
void Level::reset(){
	Ball* b = new Ball(tee, 1);
	delete physicsObjects[0];
	physicsObjects[0] = b;
	ball = b;
}

//toggle debug option for rendering
void Level::toggleDebug(){
	debug = !debug;
}
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Tile.h"
#include "Level.h"

using namespace std;

Level::Level(int number, std::string file){
	levelNumber = number;
	tee = nullptr;
	hole = nullptr;
	levelData.open(file);
	get_input();
	levelData.close();
}

Level::~Level(){

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
		t.renderTile();
	}
	hole->renderCup();
	tee->renderTee();
	
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
			/*level->*/addCup(newCup);
			break;
	
			case(2):
			int tileIndex;
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			float tempx, tempy, tempz;
			if(!(iss >> tempx)) { errorExit(linecount); }
			if(!(iss >> tempy)) { errorExit(linecount); }
			if(!(iss >> tempz)) { errorExit(linecount); }
			newTee = new Tee(tileIndex, tempx, tempy, tempz);
			/*level->*/addTee(newTee);
			break;
	
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
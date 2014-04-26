#include <vector>
#include <iostream>
#include "Tile.h"
#include "Level.h"

Level::Level(int number){
	levelNumber = number;
	tee = nullptr;
	hole = nullptr;
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
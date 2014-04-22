#include <vector>
#include <iostream>
#include "Tile.h"
#include "Level.h"

Level::Level(int number){

}

Level::~Level(){

}

void Level::render(){
	if(tiles.size() == 0){
		std::cout << "Tiles in level " << levelNumber << " is empty" << std::endl;
		return;
	}
	
	for(Tile &t: tiles){
		t.renderTile();
	}
	hole->renderCup();
	tee->renderTee();
	
}

void Level::addCup(Cup* c){
	hole = c;
}

void Level::addTee(Tee* t){
	tee = t;
}
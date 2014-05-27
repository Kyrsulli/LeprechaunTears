#include <cstdlib>
#include <glut.h>
#include <glm.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include "LeprechaunTears.h"
#include "LTObject.h"


using namespace std;


LeprechaunTears::LeprechaunTears(std::vector<LTObject*> l){
	levels = l;
	currentLevel = 0;
	camx = camy = camz = 3;
	targetx = targety = targetz = 0;
	//cameraMode = thirdperson;
	srand(time(NULL));
}

LeprechaunTears::~LeprechaunTears(){
	for(int i = 0; i < levels.size(); i++){
		delete levels[i];
	}
}

void LeprechaunTears::update(bool& changed){
	levels[currentLevel]->update();
	if(levels[currentLevel]->levelComplete()){
		nextLevel();
		changed = true;
	}
}

void LeprechaunTears::draw(){
	levels[currentLevel]->render();
}

void LeprechaunTears::nextLevel(){
	if(currentLevel + 1 == levels.size()){
		cout << "Error: no more levels" << endl;
		return;
	}
	++currentLevel;
}

void LeprechaunTears::jumpToLevel(int l){
	--l;
	if(l >= levels.size() || l < 0){
		cout << "Level Index Error: range of levels is from 1 to " << levels.size() << endl;
		return;
	}
	currentLevel = l;
}
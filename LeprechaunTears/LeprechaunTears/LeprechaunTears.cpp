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
	currentLevel = 0;
	camx = camy = camz = 3;
	targetx = targety = targetz = 0;
	//cameraMode = thirdperson;
	srand(time(NULL));
	printMenu();
}

LeprechaunTears::~LeprechaunTears(){
	for(int i = 0; i < levels.size(); i++){
		delete levels[i];
	}
}

void LeprechaunTears::update(){
	levels[currentLevel]->update();
	if(levels[currentLevel]->levelComplete()){
		nextLevel();
	}
}

void LeprechaunTears::draw(float& xr, float& yr, float& zr){
	setCameraLocation(xr, yr, zr);
	levels[currentLevel]->render();
}

void LeprechaunTears::nextLevel(){
	/*
	++currentLevel;
	if(currentLevel == levelNames.size()){
		cout << "Error: no more levels" << endl;
		--currentLevel;
		return;
	}
	delete level;
	level = new Level(currentLevel, levelNames[currentLevel]);
	courseTotal += currentHole;
	currentHole = 0;*/
}

void LeprechaunTears::resetLevel(){
	//delete levels[currentLevel];
	//levels[currentLevel] = new Level(currentLevel, levelNames[currentLevel]);
}

void LeprechaunTears::printMenu(){
	cout << endl << "Menu:" << endl
		 << "m: print this menu" << endl
		 << "l: next level" << endl
		 << "w, s: adjust power level of the ball" << endl
		 << "a, d: adjust angle of the shot" << endl
		 << "f: apply force to ball" << endl
		 //<< "Q, q, W, w, E, e: move camera" << endl
		 //<< "A, a, S, s, D, d: move target" << endl
		 << "1: Free camera mode" << endl
		 << "2: 3rd person camera mode" << endl
		 << "3: Top-down camera mode" << endl
		 << "X, x: rotate world about x axis"<< endl
		 << "Y, y: rotate world about y axis"<< endl
		 << "Z, z: rotate world about z axis"<< endl
		 << "Note: y axis is the vertical axis" << endl
		 << "Right click on the left and right side of the screen to rotate" << endl
		 << "r: reset level" << endl
		 << "b: berate the programmers for shoddy physics" << endl
		 << "c: close this program" << endl;
}

void LeprechaunTears::setCameraLocation(float& xRotate, float& yRotate, float& zRotate){
	float* cupLoc;
	float* ballLoc;
	switch(cameraMode){
	case free:
		camx = 3;
		camy = 3;
		camz = 3;
		targetx = 0;
		targety = 0;
		targetz = 0;
		break;
	case top:
		xRotate = yRotate = zRotate = 0;
		ballLoc = levels[currentLevel]->getBallLocation();
		targetx = ballLoc[0];
		targety = ballLoc[1];
		targetz = ballLoc[2];
		camx = ballLoc[0] + 0.0001;
		camy = ballLoc[1] + 7;
		camz = ballLoc[2] + 0.0001;
		delete ballLoc;
		break;
	case thirdperson:
		xRotate = yRotate = zRotate = 0;
		cupLoc = levels[currentLevel]->getCupLocation();
		ballLoc = levels[currentLevel]->getBallLocation();
		targetx = cupLoc[0];
		targety = cupLoc[1];
		targetz = cupLoc[2];
		camx = ballLoc[0] + -0.1*(cupLoc[0] - ballLoc[0]);
		camy = ballLoc[1] + 0.2;
		camz = ballLoc[2] + -0.1*(cupLoc[2] - ballLoc[2]);
		delete cupLoc;
		delete ballLoc;
	}
	gluLookAt(camx, camy,camz,             //location
			  targetx, targety, targetz,   //target
			  0,1,0);                      //up

}

void LeprechaunTears::jumpToLevel(int l){

}
#include <cstdlib>
#include <glut.h>
#include <glm.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include "LeprechaunTears.h"


using namespace std;

void LeprechaunTears::printMenu(){
	cout << endl << "Menu:" << endl
		 << "m: print this menu" << endl
		 << "l: next level" << endl
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

LeprechaunTears::LeprechaunTears(int argi, char* argv[]){
	ySpinDir = xSpinDir = 0;
	currentLevel = 0;
	for(int i = 1; i < argi; i++){
		levelNames.push_back(argv[i]);
	}
	level = new Level(currentLevel, levelNames[currentLevel]);
	camx = camy = camz = 3;
	targetx = targety = targetz = 0;
	xRotate = yRotate = zRotate = 0;
	//cameraMode = thirdperson;
	srand(time(NULL));
}

LeprechaunTears::~LeprechaunTears(){
	delete level;
	while(!levelNames.empty()){
		levelNames.pop_back();
	}
}

void LeprechaunTears::update(){
	yRotate += ySpinDir;
	//xRotate += xSpinDir;
	level->update();
	if(level->levelComplete()){
		nextLevel();
	}
}

void LeprechaunTears::draw(){
	setCameraLocation();
	glPushMatrix();{
		glRotatef(xRotate, 1, 0, 0);
		glRotatef(yRotate, 0, 1, 0);
		glRotatef(zRotate, 0, 0, 1);
		level->render();
	}glPopMatrix();
}

void LeprechaunTears::nextLevel(){
	++currentLevel;
	if(currentLevel == levelNames.size()){
		cout << "Error: no more levels" << endl;
		--currentLevel;
		return;
	}
	delete level;
	level = new Level(currentLevel, levelNames[currentLevel]);
}

void LeprechaunTears::resetLevel(){
	delete level;
	level = new Level(currentLevel, levelNames[currentLevel]);
}

void LeprechaunTears::keyboard(unsigned char key, int x, int y){
		switch(key){
	case 'd':
		level->changeAngle(-3);
		break;
	case 'a':
		level->changeAngle(3);
		break;
	case 'w':
		level->changeMag(1);
		break;
	case 's':
		level->changeMag(-1);
		break;
/*	case 'q':
		camx++;
		break;
	case 'Q':
		camx--;
		break;
	case 'w':
		camy++;
		break;
	case 'W':
		camy--;
		break;
//	case 'e':
//		camz++;
//		break;
	case 'E':
		camz--;
		break;
	case 'a':
		targetx++;
		break;
	case 'A':
		targetx--;
		break;
	case 's':
		targety++;
		break;
	case 'S':
		targety--;
		break;
//	case 'd':
//		targetz++;
//		break;
	case 'D':
		targetz--;
		break;*/
	case '1': ;
		cameraMode = free;
		break;
	case '2': ;
		cameraMode = thirdperson;
		break;
	case '3':
		cameraMode = top;
		break;
	case 'm':
		printMenu();
		break;
	case 'x':
		xRotate += 0.5;
		break;
	case 'X':
		xRotate -= 0.5;
		break;
	case 'y':
		yRotate += 0.5;
		break;
	case 'Y':
		yRotate -= 0.5;
		break;
	case 'z':
		zRotate += 0.5;
		break;
	case 'Z':
		zRotate -= 0.5;
		break;
	case 'c':
		exit(0);
	case 'l':
		nextLevel();
		break;
	case 'f':
		level->addForce();
		break;
	case 'r':
		resetLevel();
		break;
	case 'b':
		cout << "We feel shame" << endl;
		break;
		
	}
}

void LeprechaunTears::mouseClick(int button, int state, int x, int y){
	//if(cameraMode != free){ return; }
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		if(x > 400)//GLUT_WINDOW_WIDTH - 100)
			ySpinDir = 1;
		else if(x < 300)
			ySpinDir = -1;
		if(y > 400)
			xSpinDir = 1;
		else if(y < 300)
			xSpinDir = -1;
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
		ySpinDir = 0;
		xSpinDir = 0;
	}
}

void LeprechaunTears::mouseMove(int x, int y){

}

/*
glm::vec3 LeprechaunTears::getForce(){
	cout << "Enter x component of force: ";
	float x;
	cin >> x;
	cout << "Enter z component of force: ";
	float z;
	cin >> z;
	return glm::vec3(x, 0, z);
}
*/

void LeprechaunTears::setCameraLocation(){
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
		ballLoc = level->getBallLocation();
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
		cupLoc = level->getCupLocation();
		ballLoc = level->getBallLocation();
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
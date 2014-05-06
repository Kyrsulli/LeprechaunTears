#include <cstdlib>
#include <glut.h>
#include <string>
#include "LeprechaunTears.h"


using namespace std;

void LeprechaunTears::printMenu(){
	cout << "Menu:" << endl << endl
		 << "m: print this menu" << endl
		 << "l: next level" << endl
		 << "Q, q, W, w, E, e: move camera" << endl
		 << "A, a, S, s, D, d: move target" << endl
		 << "X, x: rotate world about x axis"<< endl
		 << "Y, y: rotate world about y axis"<< endl
		 << "Z, z: rotate world about z axis"<< endl
		 << "Note: y axis is the vertical axis" << endl
		 << "c: close this program" << endl;
}

LeprechaunTears::LeprechaunTears(){
	ySpinDir = xSpinDir = 0;
	currentLevel = 0;
	levelNames.push_back("hole.00.db");
	levelNames.push_back("hole.01.db");
	levelNames.push_back("hole.02.db");
	levelNames.push_back("testcourse1.db");
	levelNames.push_back("testcourse2.db");
	levelNames.push_back("testcourse3.db");
	level = new Level(currentLevel, levelNames[currentLevel]);
	camx = camy = camz = 3;
	targetx = targety = targetz = 0;
	xRotate = yRotate = zRotate = 0;
}

LeprechaunTears::~LeprechaunTears(){

}

void LeprechaunTears::update(){
	yRotate += ySpinDir;
	//xRotate += xSpinDir;
	
}

void LeprechaunTears::draw(){
	gluLookAt(camx, camy,camz,  //location
			  targetx, targety, targetz,   //target
			  0,1,0);  //up
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

void LeprechaunTears::keyboard(unsigned char key, int x, int y){
		switch(key){
	case 'q':
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
	case 'e':
		camz++;
		break;
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
	case 'd':
		targetz++;
		break;
	case 'D':
		targetz--;
		break;
	case '1': ;
		/*
		float* camLoc = level->getCupLocation();
		float* targetLoc = level->getTeeLocation();
		camx = camLoc[0];
		camy = camLoc[1] + 10;
		camz = camLoc[2];
		targetx = targetLoc[0];
		targety = targetLoc[1];
		targetz = targetLoc[2];
		break;
		*/
	case '2': ;
		/*
		float* camLoc = level->getTeeLocation();
		float* targetLoc = level->getCupLocation();
		camx = camLoc[0];
		camy = camLoc[1] + 10;
		camz = camLoc[2];
		targetx = targetLoc[0];
		targety = targetLoc[1];
		targetz = targetLoc[2];
		break;
		*/
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
	}
}

void LeprechaunTears::mouseClick(int button, int state, int x, int y){
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		if(x > 400)
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
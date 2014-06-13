#include <cstdlib>
#include <glut.h>
#include <glm.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include "LeprechaunTears.h"
#include "LTObject.h"


using namespace std;

//constructor.  Needs list of levels, and the size of the screen it is rendering to
LeprechaunTears::LeprechaunTears(std::vector<LTObject*> l, int width, int height){
	levels = l;
	w = width;
	h = height;
	currentLevel = 0;
	camx = camy = camz = 3;
	targetx = targety = targetz = 0;
	srand(time(NULL));
}

//clean up each level
LeprechaunTears::~LeprechaunTears(){
	for(int i = 0; i < levels.size(); i++){
		delete levels[i];
	}
}

//update the levels, draw their gui method, and if the level is complete, progress
void LeprechaunTears::update(bool& changed){
	levels[currentLevel]->update();
	levels[currentLevel]->onGUI();
	if(levels[currentLevel]->levelComplete()){
		nextLevel();
		changed = true;
	}
}

//render the current level
void LeprechaunTears::draw(){
	levels[currentLevel]->render();
}

//attempt to progress to the next level, stop if this would cause index out of bounds issues
void LeprechaunTears::nextLevel(){
	if(currentLevel + 1 == levels.size()){
		cout << "Error: no more levels" << endl;
		return;
	}
	++currentLevel;
}

//attempt to jump to level, first checking if it is an index that exists
//indexes are 0 - (n-1), but takes in numbers 1 - n
void LeprechaunTears::jumpToLevel(int l){
	--l;
	if(l >= levels.size() || l < 0){
		cout << "Level Index Error: range of levels is from 1 to " << levels.size() << endl;
		return;
	}
	currentLevel = l;
}

//render to screen, in addition to any level's onGUI
//only function dependant on GLUT, so another windowing
//system can be used and draw fonts that way instead
void LeprechaunTears::OnGUI(string s, int x, int y){
	glDisable(GL_TEXTURE_2D); 
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	void * font = GLUT_BITMAP_9_BY_15;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
	  char c = *i;
	  glColor3f(1.0, 1.0, 1.0);
	  glutBitmapCharacter(font, c);
	}
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}
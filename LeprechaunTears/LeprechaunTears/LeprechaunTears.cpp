#include <cstdlib>
#include <glut.h>
#include <glm.hpp>
#include <string>
#include <iostream>
#include <ctime>
#include "LeprechaunTears.h"
#include "LTObject.h"


using namespace std;


LeprechaunTears::LeprechaunTears(std::vector<LTObject*> l, int width, int height){
	levels = l;
	w = width;
	h = height;
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
	levels[currentLevel]->onGUI();
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
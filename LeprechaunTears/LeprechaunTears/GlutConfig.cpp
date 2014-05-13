#include <iostream>
#include <glut.h>
#include "util.h"
#include "LeprechaunTears.h"

using namespace std;

LeprechaunTears* engine;


void cb_display() {
	engine->display();
}

void cb_idle() {
	glutPostRedisplay();
}

void cb_reshape(int w, int h) {
	engine->reshape(w, h);
}

void cb_mouseclick(int button, int state, int x, int y) {
	engine->mouseClick(button, state, x, y);
}

void cb_mousemove(int x, int y) {
	engine->mouseMove(x, y);
}

void cb_keyboard(unsigned char key, int x, int y){
	engine->keyboard(key, x, y);
}


void setupGlut(int& argc, char* argv[]){
	engine = new LeprechaunTears(argc, argv);
	glutMotionFunc(cb_mousemove);
	glutMouseFunc(cb_mouseclick);
	glutDisplayFunc(cb_display);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);
	glutKeyboardFunc(cb_keyboard);

	
}
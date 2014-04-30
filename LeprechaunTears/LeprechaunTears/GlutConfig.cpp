#include <iostream>
#include <glut.h>
#include "util.h"
#include "Level.h"

using namespace std;

Level* level;


float camx = 3, 
	camy = 3, 
	camz = 3, 
	targetx = 0, 
	targety = 0, 
	targetz = 0;

float xRotate = 0, yRotate = 0, zRotate = 0;

void printMenu(){
	cout << "Menu:" << endl << endl
		 << "m: print this menu" << endl
		 << "Q, q, W, w, E, e: move camera" << endl
		 << "A, a, S, s, D, d: move target" << endl
		 << "X, x: rotate world about x axis"<< endl
		 << "Y, y: rotate world about y axis"<< endl
		 << "Z, z: rotate world about z axis"<< endl
		 << "Note: y axis is the vertical axis" << endl
		 << "c: close this program" << endl;
}

void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_axis(4.0);
	gluLookAt(camx, camy,camz,  //location
			  targetx, targety, targetz,   //target
			  0,1,0);  //up
	glPushMatrix();{
		glRotatef(xRotate, 1, 0, 0);
		glRotatef(yRotate, 0, 1, 0);
		glRotatef(zRotate, 0, 0, 1);
		level->render();
	}glPopMatrix();
	glFlush();
	glutSwapBuffers(); // for smoother animation
}

void cb_idle() {
	glutPostRedisplay();
}

void cb_reshape(int w, int h) {
	int aspect = w/h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, aspect, 0.5, 100); // necessary to preserve aspect ratio
	glMatrixMode(GL_MODELVIEW);
}

void cb_mouseclick(int button, int state, int x, int y) {
	//void
}

void cb_mousemove(int x, int y) {
	//void
}

void cb_keyboard(unsigned char key, int x, int y){
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
	}
}


void setupGlut(int& argc, char* argv[]){
	glutInit(&argc, argv);
	

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Cody and Kyle's Bitchin' Mini Golf game");
	glutIgnoreKeyRepeat(false);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glCullFace(GL_FRONT_AND_BACK);
	//lighting
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//GLfloat position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glutMotionFunc(cb_mousemove);
	glutMouseFunc(cb_mouseclick);
	glutDisplayFunc(cb_display);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);
	glutKeyboardFunc(cb_keyboard);

	glClearColor(0, 0, 0, 0);

	level = new Level(1, argv[1]);
	printMenu();
}
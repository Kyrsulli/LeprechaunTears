#include <cstdlib>
#include <cstdio>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

#include "util.h"


void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_axis(4.0);
	//drawTerrain();
	//drawSnowmen();
	//drawTexture();
	glFlush();
	glutSwapBuffers(); // for smoother animation
}

void cb_idle() {
	//drawTerrain();	
	//drawTexture();
	glutPostRedisplay();
}

void cb_reshape(int w, int h) {
	//windowWidth = w;
	//windowHeight = h;
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

	}
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Lab 4: Integrated Animal and Terrain");
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

	glClearColor(18.0/255.0,148.0/255.0,255.0/255.0,0); // set background to blue

	glutMainLoop();

	return 0;
}
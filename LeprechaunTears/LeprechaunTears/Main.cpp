#include <cstdlib>
#include <cstdio>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

#include "util.h"
#include "Tile.h"

using namespace std;

Tile* testTile;

void test_tile_setup(){
	//tile 1 4 
	//-0.5 0 1
	//0.5 0 1 
	//0.5 0 0
	//-0.5 0 0
	//0 0 3 0
	testTile = new Tile(1, nullptr);
	//normally these next 4 lines would be done in a for loop
	testTile->addVertex(-0.5, 0, 1);
	testTile->addVertex(0.5, 0, 1);
	testTile->addVertex(0.5, 0, 0);
	testTile->addVertex(-0.5, 0, 0);
	//also in a for loop, but different from the last one
	testTile->addNeighbor(0);
	testTile->addNeighbor(0);
	testTile->addNeighbor(3);
	testTile->addNeighbor(0);
}

void test_tile_render(){
	testTile->renderTile();
}

void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_axis(4.0);
	//drawTerrain();
	//drawSnowmen();
	//drawTexture();
	gluLookAt(10,10,10,  //location
			  0,0,0,   //target
			  0,1,0);  //up
	test_tile_render();
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

	//glClearColor(18.0/255.0,148.0/255.0,255.0/255.0,0); // set background to blue
	glClearColor(0, 0, 0, 0);

	test_tile_setup();
	cout <<"DEBUG: remove test_tile_render() in function main asap" << endl;
	cout<< "DEBUG: remove test_tile_render() in funcion cb_display asap" << endl;
	glutMainLoop();

	return 0;
}
#include <cstdlib>
#include <cstdio>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "util.h"
#include "Tile.h"
#include "Level.h"

using namespace std;

ifstream levelData;
Tile* testTile;
Level* level;
std::vector<Tile*> tileList;

std::string line;

float camx = 10, 
	camy = 10, 
	camz = 10, 
	targetx = 0, 
	targety = 0, 
	targetz = 0;

void test_tile_setup(){
	//tile 1 4 
	//-0.5 0 1
	//0.5 0 1 
	//0.5 0 0
	//-0.5 0 0
	//0 0 3 0
	testTile = new Tile(1);
	//normally these next 4 lines would be done in a for loop
	testTile->addVertex(-0.5f, 0, 1);
	testTile->addVertex(0.5f, 0, 1);
	testTile->addVertex(0.5f, 0, 0);
	testTile->addVertex(-0.5f, 0, 0);
	//also in a for loop, but different from the last one
	testTile->addNeighbor(0);
	testTile->addNeighbor(0);
	testTile->addNeighbor(3);
	testTile->addNeighbor(0);
}

void get_input(){
	int linecount = 0;
	while (!levelData.eof())
	{
		std::getline(levelData, line);
		std::istringstream iss(line);
		std::string type;
		int type_int = 0;
		if (!(iss >> type)) { break; } // error
		if(type.compare("tile") == 0) type_int = 0;
		if(type.compare("cup") == 0) type_int = 1;
		if(type.compare("tee") == 0) type_int = 2;
		Tile* newTile;

		switch(type_int){
		case(0):
		int tileIndex;
		if(!(iss >> tileIndex)) { break; }
		int edgeCount;
		if(!(iss >> edgeCount)) { printf("broke out");break; }
		float tempx;
		float tempy;
		float tempz;
		newTile = new Tile(tileIndex);
		for(int i = 0; i < edgeCount; i++){
			if(!(iss >> tempx)) { break; }
			if(!(iss >> tempy)) { break; }
			if(!(iss >> tempz)) { break; }
			newTile->addVertex(tempx, tempy, tempz);
		}
		
		int tempEdge;
		for(int i = 0; i < edgeCount; i++){
			if(!(iss >> tempEdge)) { break; }
			newTile->addNeighbor(tempEdge);

		}
		tileList.push_back(newTile);
		break;

		case(1):
		break;

		case(2):
		break;
	}
		linecount++;

 // process pair (a,b)
	}
	//put list of tiles into level
}

void test_tile_render(){
	testTile->renderTile();
	glutWireSphere(10, 10, 10);
}

void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_axis(4.0);
	//drawTerrain();
	//drawSnowmen();
	//drawTexture();
	gluLookAt(camx, camy,camz,  //location
			  targetx, targety, targetz,   //target
			  0,1,0);  //up
	
	//test_tile_render();
	for(int i = 0; i < tileList.size(); i++){
		tileList[i]->renderTile();
	}
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
	}
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	
	levelData.open(argv[1]);
	get_input();
	levelData.close();

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
	cout << "DEBUG: remove test_tile_render() in function main asap" << endl;
	cout << "DEBUG: remove test_tile_render() in funcion cb_display asap" << endl;
	glutMainLoop();

	return 0;
}
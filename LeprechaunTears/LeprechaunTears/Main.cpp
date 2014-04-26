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
Level* level;
std::vector<Tile*> tileList;

std::string line;

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

void errorExit(int i){
	//printf("Error on line %d. Press any key followed by Enter to exit.", i);
	cout << "Error on line " << i << ". Press Enter to exit." << flush;
	cin.ignore( numeric_limits <streamsize>::max(), '\n');
	exit(1);
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
		Cup* newCup;
		Tee* newTee;
		int tileIndex;
		int edgeCount;
		float tempx, tempy, tempz;

		switch(type_int){

			case(0):
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			if(!(iss >> edgeCount)) { errorExit(linecount); }
			newTile = new Tile(tileIndex);
			for(int i = 0; i < edgeCount; i++){
				if(!(iss >> tempx)) { errorExit(linecount); }
				if(!(iss >> tempy)) { errorExit(linecount); }
				if(!(iss >> tempz)) { errorExit(linecount); }
				newTile->addVertex(tempx, tempy, tempz);
			}
		
			int tempEdge;
			for(int i = 0; i < edgeCount; i++){
				if(!(iss >> tempEdge)) { errorExit(linecount); }
				newTile->addNeighbor(tempEdge);
	
			}
			tileList.push_back(newTile);
			break;

			case(1):
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			if(!(iss >> tempx)) { errorExit(linecount); }
			if(!(iss >> tempy)) { errorExit(linecount); }
			if(!(iss >> tempz)) { errorExit(linecount); }
			newCup = new Cup(tileIndex, tempx, tempy, tempz);
			level->addCup(newCup);
			break;
	
			case(2):
			int tileIndex;
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			float tempx, tempy, tempz;
			if(!(iss >> tempx)) { errorExit(linecount); }
			if(!(iss >> tempy)) { errorExit(linecount); }
			if(!(iss >> tempz)) { errorExit(linecount); }
			newTee = new Tee(tileIndex, tempx, tempy, tempz);
			level->addTee(newTee);
			break;
	
			default:
			errorExit(linecount);
			break;
		}
		linecount++;

 // process pair (a,b)
	}
	level->addTiles(tileList);
	//put list of tiles into level
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
	//for(int i = 0; i < tileList.size(); i++){
	//	tileList[i]->renderTile();
	//}
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

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	level = new Level(1);
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

	printMenu();
	glutMainLoop();

	return 0;
}
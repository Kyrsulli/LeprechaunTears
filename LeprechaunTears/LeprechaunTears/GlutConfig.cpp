#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glut.h>
#include "Level.h"
#include "Tile.h"
#include "util.h"
#include "LeprechaunTears.h"

using namespace std;

enum CAMERAMODES{freeCamera, thirdpersonCamera, topCamera};

int currentHoleScore = 0;
int totalCourseScore = 0;

LeprechaunTears* engine;
int ySpinDir;
int xSpinDir;
float xRotate, yRotate, zRotate;


void errorExit(int i){
	//printf("Error on line %d. Press any key followed by Enter to exit.", i);
	cout << "Error on line " << i << ". Press Enter to exit." << flush;
	cin.ignore( numeric_limits <streamsize>::max(), '\n');
	exit(1);
}

std::vector<LTObject*> readLevels(char* courseData){
	//cout << "Kyle, move your modified level reading code to GlutConfig.cpp in function readLevels near the top" << endl;
	//cout << "Create the levels and put them into this vector in order, Level now inherits from LTObject" << endl;
	//cout << "Eventually I will make a menu/score screen to be inserted before/after all the levels that extends this class too" << endl;
	vector<LTObject*> levels;
	int linecount = 0;
	int levelCount = 0;
	int currentcount = 0;
	string courseName;
	string line;
	fstream levelData;
	levelData.open(courseData);
	std::getline(levelData, line);
	std::istringstream iss(line);
	std::string type;
	if(!(iss >> type)) {errorExit(linecount); }
	if(type.compare("course") != 0) {errorExit(linecount); }
	//This is where the name of the whole course is assigned.
	if(!(iss >> courseName)) {errorExit(linecount); }
	if(!(iss >> levelCount)) {errorExit(linecount); }
	//I have to set up this level pointer here, otherwise it will complain.
	Level* currLevel = new Level(currentcount, "");
	vector<Tile*> tileList;
	while (!levelData.eof())
	{
		//cout << "blarghehonk" << endl;
		std::getline(levelData, line);
		std::istringstream iss(line);
		std::string type;
		int p;
		int type_int = 0;
		if (!(iss >> type)) { break; } // error
		if(type.compare("begin_hole") == 0){
			currLevel = new Level(currentcount, "");
			continue;
		}
		if(type.compare("end_hole") == 0){
			currentcount++;
			currLevel->addTiles(tileList);
			tileList.clear();
			levels.push_back(currLevel);
			continue;
		}
		if(type.compare("tile") == 0) type_int = 0;
		if(type.compare("cup") == 0) type_int = 1;
		if(type.compare("tee") == 0) type_int = 2;
		if(type.compare("name") == 0) type_int = 3;
		if(type.compare("par") == 0) type_int = 4;
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
			currLevel->addCup(newCup);
			break;
	
			case(2):
			int tileIndex;
			if(!(iss >> tileIndex)) { errorExit(linecount); }
			float tempx, tempy, tempz;
			if(!(iss >> tempx)) { errorExit(linecount); }
			if(!(iss >> tempy)) { errorExit(linecount); }
			if(!(iss >> tempz)) { errorExit(linecount); }
			newTee = new Tee(tileIndex, tempx, tempy, tempz);
			currLevel->addTee(newTee);
			break;

			case(3):
				if(!(iss >> currLevel->name)) { errorExit(linecount); }
			break;

			case(4):
				if(!(iss >> p)) { errorExit(linecount); }
				currLevel->setPar(p);
			break;
	
			default:
			errorExit(linecount);
			break;
		}
		linecount++;

 // process pair (a,b)
	}
	//printf("%d, %d\n", currentcount, levelCount);
	//put list of tiles into level
	//level reading goes here
	return levels;
}

void DrawHUD(){
	cout << "Drawing HUD on line 151 of GlutConfig.cpp not implemented" << endl;
}

void printMenu(){
	cout << endl << "Menu:" << endl
		 << "m: print this menu" << endl
		 << "l: next level" << endl
		 << "w, s: adjust power level of the ball" << endl
		 << "a, d: adjust angle of the shot" << endl
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
		 << "j: jump to level (move to command prompt window to enter value)" << endl
		 << "c: close this program" << endl;
}

void setCameraLocation(){
	float* cupLoc;
	float* ballLoc;
	switch(engine->cameraMode){
	case freeCamera:
		engine->camx = 3;
		engine->camy = 3;
		engine->camz = 3;
		engine->targetx = 0;
		engine->targety = 0;
		engine->targetz = 0;
		break;
	case topCamera:
		xRotate = yRotate = zRotate = 0;
		ballLoc = static_cast<Level*>(engine->levels[engine->currentLevel])->getBallLocation();
		engine->targetx = ballLoc[0];
		engine->targety = ballLoc[1];
		engine->targetz = ballLoc[2];
		engine->camx = ballLoc[0] + 0.0001;
		engine->camy = ballLoc[1] + 7;
		engine->camz = ballLoc[2] + 0.0001;
		delete ballLoc;
		break;
	case thirdpersonCamera:
		xRotate = yRotate = zRotate = 0;
		cupLoc = static_cast<Level*>(engine->levels[engine->currentLevel])->getCupLocation();
		ballLoc = static_cast<Level*>(engine->levels[engine->currentLevel])->getBallLocation();
		engine->targetx = cupLoc[0];
		engine->targety = cupLoc[1];
		engine->targetz = cupLoc[2];
		engine->camx = ballLoc[0] + -0.1*(cupLoc[0] - ballLoc[0]);
		engine->camy = ballLoc[1] + 0.2;
		engine->camz = ballLoc[2] + -0.1*(cupLoc[2] - ballLoc[2]);
		delete cupLoc;
		delete ballLoc;
	}
	gluLookAt(engine->camx, engine->camy, engine->camz,             //location
			  engine->targetx, engine->targety, engine->targetz,   //target
			  0,1,0);                      //up

}

void cb_display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	draw_axis(4.0);
	bool engineChangedLevel = false;
	engine->update(engineChangedLevel);
	if(engineChangedLevel){
		totalCourseScore += currentHoleScore;
		currentHoleScore = 0;
	}
	glPushMatrix();{
		glRotatef(xRotate, 1, 0, 0);
		glRotatef(yRotate, 0, 1, 0);
		glRotatef(zRotate, 0, 0, 1);
		setCameraLocation();
		engine->draw();
	}glPopMatrix();
	DrawHUD();
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
	gluPerspective(90.0, aspect, 0.0005, 100); // necessary to preserve aspect ratio
	glMatrixMode(GL_MODELVIEW);
}

void cb_mouseclick(int button, int state, int x, int y) {
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

void cb_mousemove(int x, int y) {
	//engine->mouseMove(x, y);
}

void cb_keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'd':
		static_cast<Level*>(engine->levels[engine->currentLevel])->changeAngle(-3);
		break;
	case 'a':
		static_cast<Level*>(engine->levels[engine->currentLevel])->changeAngle(3);
		break;
	case 'w':
		static_cast<Level*>(engine->levels[engine->currentLevel])->changeMag(1);
		break;
	case 's':
		static_cast<Level*>(engine->levels[engine->currentLevel])->changeMag(-1);
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
		engine->cameraMode = freeCamera;
		break;
	case '2': ;
		engine->cameraMode = thirdpersonCamera;
		break;
	case '3':
		engine->cameraMode = topCamera;
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
		static_cast<Level*>(engine->levels[engine->currentLevel])->reset();
		engine->nextLevel();
		currentHoleScore = 0;
		break;
	case 'f':
		static_cast<Level*>(engine->levels[engine->currentLevel])->addForce();
		currentHoleScore++;
		break;
	case 'r':
		static_cast<Level*>(engine->levels[engine->currentLevel])->reset();
		currentHoleScore = 0;
		break;
	case 'b':
		cout << "We feel shame as a minor action." << endl;
		break;
	case 'j':
		cout << "Enter level to jump to: ";
		int foo;
		cin >> foo;
		engine->jumpToLevel(foo);
		break;
	}
}


void setupGlut(int& argc, char* argv[]){
	glutInit(&argc, argv);
	engine = new LeprechaunTears(readLevels(argv[1]));

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

	glClearColor(0.64f/255.0f, 76.0f/255.0f, 1, 0);
	glutMotionFunc(cb_mousemove);
	glutMouseFunc(cb_mouseclick);
	glutDisplayFunc(cb_display);
	glutReshapeFunc(cb_reshape);
	glutIdleFunc(cb_idle);
	glutKeyboardFunc(cb_keyboard);

	printMenu();
}
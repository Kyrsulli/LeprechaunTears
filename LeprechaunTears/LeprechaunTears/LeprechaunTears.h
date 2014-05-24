#include <iostream>
#include <string>
#include <vector>
#include "Level.h"
#include "LTObject.h"

class LeprechaunTears{
public:
	LeprechaunTears(std::vector<LTObject*>);
	~LeprechaunTears();
	void update();
	void draw(float&, float&, float&);
	void nextLevel();
	void jumpToLevel(int);
	void resetLevel();
	//void keyboard(unsigned char key, int x, int y);
	//void mouseClick(int button, int state, int x, int y);
	//void mouseMove(int x, int y);
	//void display();
	//void reshape(int, int);
	void printMenu();
	int currentLevel;
	int cameraMode;
	enum CAMERAMODES{free, thirdperson, top};
	std::vector<Level*> levels;
	float camx, camy, camz, targetx, targety, targetz;
private:
	//Level* level;
	
	

	//scoring
	//int currentHole, courseTotal;

	
	//glm::vec3 getForce();
	void setCameraLocation(float&, float&, float&);
	
	//void setup();
};
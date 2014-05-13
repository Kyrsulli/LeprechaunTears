#include <iostream>
#include <string>
#include <vector>
#include "Level.h"

class LeprechaunTears{
public:
	LeprechaunTears(int, char*[]);
	~LeprechaunTears();
	void update();
	void draw();
	void keyboard(unsigned char key, int x, int y);
	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void printMenu();
private:
	int ySpinDir;
	int xSpinDir;
	Level* level;
	int currentLevel;
	int cameraMode;
	enum CAMERAMODES{free, thirdperson, top};
	std::vector<std::string> levelNames;
	float camx, camy, camz, targetx, targety, targetz;
	float xRotate, yRotate, zRotate;

	void nextLevel();
	//glm::vec3 getForce();
	void setCameraLocation();
	void resetLevel();
};
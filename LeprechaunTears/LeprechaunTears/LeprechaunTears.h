#include <iostream>
#include <string>
#include <vector>
#include "Level.h"
#include "LTObject.h"

class LeprechaunTears{
public:
	LeprechaunTears(std::vector<LTObject*>, int, int);
	~LeprechaunTears();
	void update(bool&);
	void draw();
	void nextLevel();
	void jumpToLevel(int);
	void printMenu();
	void OnGUI(std::string, int, int);
	int currentLevel;
	int cameraMode;
	std::vector<LTObject*> levels;
	float camx, camy, camz, targetx, targety, targetz;
private:
	int w, h;

};
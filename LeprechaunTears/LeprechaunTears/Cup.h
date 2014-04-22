#pragma once

class Cup{
public:
	Cup(int, float, float, float);
	~Cup();
	float x, y, z;
	int id;
	void renderCup();
};

#pragma once

class Tee{
public:
	Tee(int, float, float, float);
	~Tee();
	float x, y, z;
	int id;
	void renderTee();
};

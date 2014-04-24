#include <glut.h>
#include "Tee.h"

Tee::Tee(int ID, float xx, float yy, float zz){
	id = ID;
	x = xx;
	y = yy;
	z = zz;
}

Tee::~Tee(){

}


void Tee::renderTee(){
	glPushMatrix();{
		glTranslatef(x, y, z);
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidCone(0.5f, 0.5f, 3, 10);
	}glPopMatrix();
}
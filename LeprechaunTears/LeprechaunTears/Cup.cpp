#include <glut.h>
#include "Cup.h"

Cup::Cup(int ID, float xx, float yy, float zz){
	id = ID;
	x = xx;
	y = yy;
	z = zz;
}

Cup::~Cup(){

}

void Cup::renderCup(){
	glPushMatrix();{
		glTranslatef(x, y, z);
		glColor3f(0.0f, 0.0f, 0.0f);
		glutSolidSphere(1.0f, 25, 25);
	}glPopMatrix();
}
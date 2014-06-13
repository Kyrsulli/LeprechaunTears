#include <glut.h>
#include "Tee.h"

//constructor
Tee::Tee(int ID, float xx, float yy, float zz){
	id = ID;
	x = xx;
	y = yy;
	z = zz;
}

//destructor, nothing to clean up
Tee::~Tee(){

}

//render a cone at the tee location
void Tee::renderTee(){
	glPushMatrix();{
		glTranslatef(x, y, z);
		glColor3f(0.0f, 0.0f, 1.0f);
		glRotatef(-90, 1, 0, 0);
		glutSolidCone(0.125f, 0.02f, 25, 25);
	}glPopMatrix();
}
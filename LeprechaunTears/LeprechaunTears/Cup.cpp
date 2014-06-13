#include <glut.h>
#include "Cup.h"

//constructor
Cup::Cup(int ID, float xx, float yy, float zz){
	id = ID;
	x = xx;
	y = yy;
	z = zz;
}

//destructor, nothing to clean up
Cup::~Cup(){

}

//render a sphere where the cup is
void Cup::renderCup(){
	glPushMatrix();{
		glTranslatef(x, y, z);
		glColor3f(0.0f, 0.0f, 0.0f);
		glutSolidSphere(0.08f, 25, 25);
	}glPopMatrix();
}
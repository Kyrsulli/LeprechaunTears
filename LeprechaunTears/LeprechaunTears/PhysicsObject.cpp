#include <glm.hpp>
#include <cstdlib>
#include <glut.h>
#include <iostream>
#include "PhysicsObject.h"

using namespace std;
using namespace glm;

PhysicsObject::PhysicsObject(){

}

PhysicsObject::~PhysicsObject(){

}

void PhysicsObject::update(Tile* t){
	
}

void PhysicsObject::render(){
	
}

void PhysicsObject::addDrag(glm::vec3 foo){
	//printf("%f, %f\n", velocity.x, velocity.z);
	velocity.x -= foo.x/1000;
	velocity.z -= foo.z/1000;
	velocity *= friction;
	if(foo.x == 0 && foo.z == 0){
	if(abs(velocity.x) <= 0.005){
		velocity.x = 0;
	}
	if(abs(velocity.y) <= 0.005){
		velocity.y = 0;
	}
	if(abs(velocity.z) <= 0.005){
		velocity.z = 0;
	}
	}
}

void PhysicsObject::addForce(vec3 force){
	velocity = force;
}
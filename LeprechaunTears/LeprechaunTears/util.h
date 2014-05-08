#pragma once

#include <glm.hpp>
inline float constrain(float&, float, float);

void RotateView(float deltaX, float deltaY);

void draw_string( float x, float y, float z, char *txt );

void draw_axis(float len);

//inline double dist(glm::vec3, glm::vec3);


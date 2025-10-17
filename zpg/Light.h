#pragma once
#include "Includes.h"
#include "Subject.h"

using namespace glm;

class Light : public Subject
{
private:
	vec3 mPosition = vec3(0.0);
	vec3 mColor = vec3(0.0);
public:
	Light() {};
	Light(vec3 position, vec3 color);
	vec3 getColor();
	vec3 getPosition();
};
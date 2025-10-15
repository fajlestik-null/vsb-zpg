#pragma once
#include "Includes.h"
#include "ISubject.h"

using namespace glm;

class Light : public ISubject
{
private:
	vec3 mPosition;
	vec3 mColor;
public:
	Light() {};
	Light(vec3 position, vec3 color);
	vec3 getColor();
	vec3 getPosition();
};
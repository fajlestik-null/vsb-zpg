#pragma once
#include "Includes.h"

//using namespace glm;

class Light
{
private:
	glm::vec3 mPosition;
	glm::vec3 mColor;
public:
	Light(glm::vec3 position, glm::vec3 color);
	glm::vec3 getColor();
	//glm::vec3 getReflection();
};
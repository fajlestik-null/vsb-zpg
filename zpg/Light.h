#pragma once
#include "Includes.h"
#include "Subject.h"


class Light : public Subject
{
private:
	vec3 mPosition;
	vec3 mColor;
public:
	Light();
	Light(const vec3 POSITION, const vec3 COLOR);
	vec3 getColor() const;
	vec3 getPosition() const;
};
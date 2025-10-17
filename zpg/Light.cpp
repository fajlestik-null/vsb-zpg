#include "Light.h"

Light::Light() :mPosition(vec3(0.0)), mColor(vec3(0.0))
{
	this->notifyObservers();
}

Light::Light(const vec3 POSITION, const vec3 COLOR):mPosition(POSITION), mColor(COLOR)
{
	this->notifyObservers();
}
vec3 Light::getColor() const
{
	return this->mColor;
}
vec3 Light::getPosition() const
{
	return this->mPosition;
}


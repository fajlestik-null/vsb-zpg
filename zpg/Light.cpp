#include "Light.h"

Light::Light(vec3 position, vec3 color):mPosition(position), mColor(color)
{
	notifyObservers();
}
vec3 Light::getColor()
{
	return this->mColor;
}
vec3 Light::getPosition()
{
	return this->mPosition;
}


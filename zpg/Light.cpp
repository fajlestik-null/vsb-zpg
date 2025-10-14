#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color):mPosition(position), mColor(color)
{
}
glm::vec3 Light::getColor()
{
	return this->mColor;
}
/*glm::vec3 Light::getReflection()
{
	//testovací, nebude fungovat
	return glm::vec3(0.0);
}*/

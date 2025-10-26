#include "Light.h"

int Light::LIGHT_COUNT = 0;

SubjectType Light::getType() const
{
	return SubjectType::LIGHT;
}

Light::~Light()
{
	Light::LIGHT_COUNT--;
}

Light::Light(const vec3 LIGHT_COLOR, const float INTENSITY)
{
	this->mID = LIGHT_COUNT++;
	this->mLightColor = LIGHT_COLOR;
	this->mIntensity = INTENSITY;
}

Light::Light(Model* model, ShaderProgram* shader,const vec3 COLOR)
{
	this->setModel(model);
	this->addShaderProgram(shader);
	this->setColor(COLOR);
}

vec3 Light::getLightColor() const
{
	return this->mLightColor;
}

float Light::getIntensity() const
{
	return this->mIntensity;
}

int Light::getID() const
{
	return this->mID;
}

void Light::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
	this->getTransformManager().get()->calculateTransform();
	if (!this->getTransformManager().get()->isCalculated())
	{
	}
	this->notifyObservers();
}

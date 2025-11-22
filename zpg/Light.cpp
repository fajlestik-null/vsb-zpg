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

Light::Light(Model* model, ShaderProgram* shader,const vec3 COLOR)
{
	this->setModel(model);
	this->addShaderProgram(shader);
	this->setColor(COLOR);
	this->setTexture(nullptr);
}

Light::Light(const LightType LIGHT_TYPE, const vec3 LIGHT_COLOR, const float INTENSITY, const float DISTANCE = 1.0f) : mDistanceToRemember(0.0f)
{
	this->mID = LIGHT_COUNT++;
	this->mLightType = LIGHT_TYPE;
	this->mLightColor = LIGHT_COLOR;
	this->mIntensity = INTENSITY;
	this->mDistance = DISTANCE;
	this->mActive = false;
	this->setTexture(nullptr);
}

vec3 Light::getLightColor() const
{
	return this->mLightColor;
}

float Light::getIntensity() const
{
	return this->mIntensity;
}

int Light::getLightType() const
{
	return (int) this->mLightType;
}

float Light::getDistance() const
{
	return this->mDistance;
}

bool Light::isActive() const
{
	return this->mActive;
}

void Light::setActive(const bool ACTIVE)
{
	this->mActive = ACTIVE;
}

int Light::getID() const
{
	return this->mID;
}

void Light::processKeyboard(GLFWwindow* window, float delta_time, Controls* controls)
{
	if (isActive() == false)
		return;

	if (controls->isKeyTriggered(GLFW_KEY_F))
	{
		// Toggle flashlight (reflector) on/off
		if (mDistance > 0.0f)
		{
			mDistanceToRemember = mDistance;
			mDistance = 0.0f;
		}
		else
		{
			mDistance = mDistanceToRemember;
			mDistanceToRemember = 0.0f;
		}

		this->notifyObservers();
	}
}

void Light::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
	this->processKeyboard(window, deltaTime, controls);
	this->getTransformManager().get()->calculateTransform();
	if (!this->getTransformManager().get()->isCalculated())
	{
		this->notifyObservers();
	}
}

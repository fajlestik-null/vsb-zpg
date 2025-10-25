#include "DrawableObject.h"

DrawableObject::DrawableObject()
{
}

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, vec3 objectColor)
{
	this->setModel(model);
	this->addShaderProgram(shaderProgram);
	this->mObjectColor = objectColor;
}

DrawableObject::~DrawableObject()
{
	this->mModel = nullptr;
	this->mShaderPrograms.clear();
}

void DrawableObject::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
	this->getTransformManager().get()->calculateTransform();
	if (!this->getTransformManager().get()->isCalculated())
	{
		this->notifyObservers();
	}
}
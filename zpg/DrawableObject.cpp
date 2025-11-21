#include "DrawableObject.h"

DrawableObject::DrawableObject()
{
}

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, vec3 objectColor)
{
	this->setModel(model);
	this->addShaderProgram(shaderProgram);
	this->mObjectColor = objectColor;
	this->setTexture(nullptr);
	this->setStencilIndex(0);
}

DrawableObject::~DrawableObject()
{
	this->mModel = nullptr;
	this->mShaderPrograms.clear();
}

void DrawableObject::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
	this->getTransformManager()->calculateTransform();
	this->notifyObservers();
}

SubjectType DrawableObject::getType() const
{
	return SubjectType::DRAWABLE_OBJECT;
}

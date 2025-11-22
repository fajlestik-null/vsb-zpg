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
	this->mMaterial = new Material();
	this->setStencilIndex(0);
	this->attach(shaderProgram);
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

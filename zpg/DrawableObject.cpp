#include "DrawableObject.h"

DrawableObject::DrawableObject() : mModel(nullptr), mTransformManager(make_shared<TransformManager>()), mColor(vec3(1.0f, 1.0f, 1.0f))
{
}

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, vec3 objectColor) : mTransformManager(make_shared<TransformManager>())
{
	this->addModel(model);
	this->addShaderProgram(shaderProgram);
	this->mColor = objectColor;
}

void DrawableObject::addModel(Model* model)
{
	delete mModel;
	mModel = model;
}
void DrawableObject::addShaderProgram(ShaderProgram* shaderProgram)
{
	mShaderPrograms.push_back(shaderProgram);
}

// Transformations
void DrawableObject::addLocalTransform(TransformBase* transformation)
{
	mTransformManager->addLocalTransform(transformation);
}

void DrawableObject::addGlobalTransform(TransformBase* transformation)
{
	mTransformManager->addGlobalTransform(transformation);
}

void DrawableObject::addStaticTransform(TransformBase * transformation)
{
	mTransformManager->addStaticTransform(transformation);
}


shared_ptr<TransformManager> DrawableObject::getTransformManager()
{
	return mTransformManager;
}


void DrawableObject::addParent(shared_ptr<TransformManager> parent)
{
	mTransformManager->addParent(parent);
}



void DrawableObject::draw()
{
	mTransformManager->calculateTransform();

	for (const auto& shader : mShaderPrograms)
	{
		shader->useShader(mTransformManager->getFinalMatrix());
	}
	mModel->put();
}
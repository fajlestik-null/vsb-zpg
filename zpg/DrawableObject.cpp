#include "DrawableObject.h"

DrawableObject::DrawableObject() : mModel(nullptr), mMatrix(1.0f), mDynamicMatrix(1.0f)
{
}

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram) : mMatrix(1.0f), mDynamicMatrix(1.0f)
{
	this->addModel(model);
	this->addShaderProgram(shaderProgram);
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
void DrawableObject::addDynamicTransformation(TransformBase* transformation)
{
	mTransformations.push_back(transformation);
	executeDynamicTrasformations();
}

void DrawableObject::addStaticTransformation(TransformBase * transformation)
{
	updateMatrix(transformation);
}

void DrawableObject::executeDynamicTrasformations()
{
	mDynamicMatrix = mat4(1.0);
	if (!mTransformations.empty())
	{
		for (auto t : mTransformations)
		{
			mDynamicMatrix = mDynamicMatrix * t->getModelMatrix();
		}
	}
}

void DrawableObject::updateMatrix(TransformBase * transformation)
{
	mMatrix = mMatrix * transformation->getModelMatrix();
}

void DrawableObject::draw()
{
	mMatrix = mMatrix * mDynamicMatrix;
	for (const auto& shader : mShaderPrograms)
	{
		shader->useShader(mMatrix);
		mModel->put();
	}
}
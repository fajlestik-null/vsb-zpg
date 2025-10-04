#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, Transformation * transformation)
{
	addModel(model);
	addShaderProgram(shaderProgram);
	updateMatrix(transformation);
}

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram)
{
	addModel(model);
	addShaderProgram(shaderProgram);
}


void DrawableObject::addModel(Model* model)
{
	mModel = model;
}
void DrawableObject::addShaderProgram(ShaderProgram* shaderProgram)
{
	mShaderPrograms.push_back(shaderProgram);
}
void DrawableObject::addTransformation(Transformation* transformation)
{
	mTransformations.push_back(transformation);
	dynamicTrasformations();
}

void DrawableObject::staticTransformation(Transformation * transformation)
{
	updateMatrix(transformation);
}

void DrawableObject::dynamicTrasformations()
{
	mDynamicMatrix = glm::mat4(1.0);
	if (!mTransformations.empty())
	{
		for (auto t : mTransformations)
		{
			mDynamicMatrix = mDynamicMatrix * t->getModelMatrix();
		}
	}
}

void DrawableObject::updateMatrix(Transformation * transformation)
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
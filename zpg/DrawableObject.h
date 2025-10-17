#pragma once
#include "ShaderProgram.h"
#include "Model.h"
#include "Transformation.h"

class DrawableObject
{
private:
	Model* mModel;
	vector<ShaderProgram*> mShaderPrograms;
	vector<TransformBase*> mTransformations;
	mat4 mMatrix;
	mat4 mDynamicMatrix;

	void executeDynamicTrasformations();

public:
	DrawableObject();
	DrawableObject(Model* model, ShaderProgram* shaderProgram);

	void addModel(Model* model);
	void addShaderProgram(ShaderProgram* shaderProgram);
	void addDynamicTransformation(TransformBase* transformation);
	void addStaticTransformation(TransformBase* transformation);
	void updateMatrix(TransformBase* transformation);

	void draw();
};
#pragma once
#include "ShaderProgram.h"
#include "Model.h"
#include "Transformation.h"

class DrawableObject
{
private:
	Model* mModel;
	std::vector<ShaderProgram*> mShaderPrograms;
	std::vector<TransformBase*> mTransformations;
	glm::mat4 mMatrix = glm::mat4(1.0f);
	glm::mat4 mDynamicMatrix = glm::mat4(1.0f);

public:
	DrawableObject(Model* model, ShaderProgram* shaderProgram, TransformBase * transformation);
	DrawableObject(Model* model, ShaderProgram* shaderProgram);

	void addModel(Model* model);
	void addShaderProgram(ShaderProgram* shaderProgram);
	void addTransformation(TransformBase* transformation);
	void staticTransformation(TransformBase* transformation);
	void dynamicTrasformations();
	void updateMatrix(TransformBase* transformation);

	void draw();
};
#pragma once
#include "ShaderProgram.h"
#include "Model.h"
#include "TransformManager.h"

class DrawableObject
{
private:
	Model* mModel;
	vector<ShaderProgram*> mShaderPrograms;
	shared_ptr<TransformManager> mTransformManager;

public:
	DrawableObject();
	DrawableObject(Model* model, ShaderProgram* shaderProgram);

	void addModel(Model* model);
	void addShaderProgram(ShaderProgram* shaderProgram);

	void addLocalTransform(TransformBase* transformation);
	void addGlobalTransform(TransformBase* transformation);
	void addStaticTransform(TransformBase* transformation);

	shared_ptr<TransformManager> getTransformManager();
	void addParent(shared_ptr<TransformManager> parent);

	void draw();
};
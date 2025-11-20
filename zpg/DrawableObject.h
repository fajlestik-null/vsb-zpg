#pragma once
#include "WorldEntity.h"

class DrawableObject : public WorldEntity
{
public:
	DrawableObject();
	DrawableObject(Model* model, ShaderProgram* shaderProgram, const vec3 OBJECT_COLOR = vec3(1.0f, 1.0f, 1.0f));

	~DrawableObject();

	void update(GLFWwindow* window, float deltaTime, Controls* controls) override; 	

	SubjectType getType() const override;
};
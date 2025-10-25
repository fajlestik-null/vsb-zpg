#pragma once
#include "WorldEntity.h"

class DrawableObject : public WorldEntity
{
public:
	DrawableObject();
	DrawableObject(Model* model, ShaderProgram* shaderProgram, const vec3 OBJECT_COLOR = vec3(0.0f, 0.5f, 0.0f));

	~DrawableObject();

	void update(GLFWwindow* window, float deltaTime, Controls* controls) override; 	

};
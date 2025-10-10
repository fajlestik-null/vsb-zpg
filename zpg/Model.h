#pragma once
//Include GLEW
#include <GL/glew.h>
#include "Includes.h"
#include <vector>

class Model
{
private:
	GLuint mVBO, mVAO;
	std::vector<float> mVertices;
public:
	Model();
	Model(const std::vector<float> &VERTICES);
	~Model();
	void put() const;
};
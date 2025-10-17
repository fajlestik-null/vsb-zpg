#pragma once
#include "Includes.h"

class Model
{
private:
	GLuint mVBO, mVAO;
	vector<float> mVertices;
public:
	Model();
	Model(const vector<float> &VERTICES);
	~Model();
	void put() const;
};
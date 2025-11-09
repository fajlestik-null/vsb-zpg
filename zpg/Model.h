#pragma once
#include "Includes.h"

class Model
{
private:
	GLuint mVBO, mVAO, mEBO;
	vector<float> mVertices;
	vector<unsigned int> mIndices;

public:
	Model();
	Model(const vector<float> &VERTICES);
	Model(const vector<float>& VERTICES, int i);
	~Model();
	void put() const;
	bool loadModelFromFile(const string& PATH);
};
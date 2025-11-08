#pragma once
#include "Includes.h"
#include "Texture.h"

class Model
{
private:
	GLuint mVBO, mVAO, mEBO;
	vector<float> mVertices;
	vector<unsigned int> mIndices;
	Texture* mTexture = nullptr;

public:
	Model();
	Model(const vector<float> &VERTICES);
	Model(const vector<float>& VERTICES, int i);
	~Model();
	void put() const;
	void setTexture(Texture* texture);
	bool loadModelFromFile(const string& PATH);
	Texture* getTexture() const;
};
#include "Model.h"

Model::Model(const std::vector<float> &VERTICES): mVertices(VERTICES)
{
	//K èemu manuánì assignujeme èísla VAO a VBO?
	glGenBuffers(1, &mVBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVAO); //generate the VAO
	glBindVertexArray(mVAO); //bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color
	glEnableVertexAttribArray(1);
}

Model::~Model()
{
	mVertices.clear();
}

void Model::put() const
{
	glBindVertexArray(mVAO);
	// draw
	glDrawArrays(GL_TRIANGLES, 0, sizeof(mVertices)/3);
}

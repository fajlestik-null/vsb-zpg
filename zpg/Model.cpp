#include "Model.h"

Model::Model(const vector<float> &VERTICES): mVertices(VERTICES)
{

	glGenBuffers(1, &mVBO); // generate the VBO id
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVAO); //generate the VAO id
	glBindVertexArray(mVAO); //bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // color

}

Model::Model() :mVAO(0), mVBO(0) {}

Model::~Model()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

void Model::put() const
{
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) mVertices.size()/6);
}

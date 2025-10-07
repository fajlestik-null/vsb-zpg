#pragma once
#include "Includes.h"

class Shader
{
private:
	GLuint mShaderID;
public:
	Shader();
	~Shader() {};
	void createShader(GLenum type, const char* shader);
	void attachShader(GLuint shaderProgramID);
};
#pragma once
#include "Includes.h"
#include <fstream>

class Shader
{
private:
	GLuint mShaderID;
public:
	Shader();
	~Shader() {};
	void createShader(GLenum type, const char* shader);
	void createShaderFromFile(GLenum shaderType, const char* shaderFile);
	void attachShader(GLuint shaderProgramID);
};
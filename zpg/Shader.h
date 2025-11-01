#pragma once
#include "Includes.h"

class Shader
{
private:
	GLuint mShaderID;
public:
	Shader();
	~Shader();
	void createShader(const GLenum SHADER_TYPE, const char* SHADER_CODE);
	bool createShaderFromFile(const GLenum SHADER_TYPE, const char* SHADER_FILE);
	bool createShaderFromFile(const GLenum SHADER_TYPE, const string& SHADER_FILE);
	void attachShader(const GLuint SHADER_PROGRAM_ID) const;
};
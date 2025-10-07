#include "Shader.h"

Shader::Shader()
{
	mShaderID = 0;
}

void Shader::createShader(GLenum type, const char* shader)
{
	mShaderID = glCreateShader(type);
	glShaderSource(mShaderID, 1, &shader, NULL);
	glCompileShader(mShaderID);
}
void Shader::attachShader(GLuint idShaderProgram)
{
	glAttachShader(idShaderProgram, mShaderID);
}

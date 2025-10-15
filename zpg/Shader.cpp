#include "Shader.h"

Shader::Shader()
{
	mShaderID = 0;
}

void Shader::createShader(GLenum shaderType, const char* shaderCode)
{
	// Creates an empty shader
	mShaderID = glCreateShader(shaderType);
	// Sets the source code of the shader.
	glShaderSource(mShaderID, 1, &shaderCode, NULL);
	// Compiles the shader source code
	glCompileShader(mShaderID);
}

void Shader::createShaderFromFile(GLenum shaderType, const char* shaderFile)
{
	//Loading the contents of a file into a variable
	std::ifstream file(shaderFile);
	if (!file.is_open())
	{
		std::cout << "Unable to open file " << shaderFile << std::endl;
		exit(-1);
	}
	std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	createShader(shaderType, shaderCode.c_str());
}

void Shader::attachShader(GLuint idShaderProgram)
{
	//Attaches the shader to the shaderProgram
	glAttachShader(idShaderProgram, mShaderID);
}

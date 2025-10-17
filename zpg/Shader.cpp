#include "Shader.h"

Shader::Shader() : mShaderID(0)
{
}

Shader::~Shader()
{
	glDeleteShader(mShaderID);
}

void Shader::createShader(const GLenum SHADER_TYPE, const char* SHADER_CODE)
{
	// Creates an empty shader
	this->mShaderID = glCreateShader(SHADER_TYPE);
	// Sets the source code of the shader.
	glShaderSource(mShaderID, 1, &SHADER_CODE, NULL);
	// Compiles the shader source code
	glCompileShader(mShaderID);
}

void Shader::createShaderFromFile(const GLenum SHADER_TYPE, const char* SHADER_FILE)
{
	//Loading the contents of a file into a variable
	ifstream file(SHADER_FILE);
	if (!file.is_open())
	{
		cout << "Unable to open file " << SHADER_FILE << endl;
		exit(-1);
	}
	string shaderCode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	this->createShader(SHADER_TYPE, shaderCode.c_str());
}

void Shader::attachShader(const GLuint SHADER_PROGRAM_ID) const
{
	//Attaches the shader to the shaderProgram
	glAttachShader(SHADER_PROGRAM_ID, mShaderID);
}

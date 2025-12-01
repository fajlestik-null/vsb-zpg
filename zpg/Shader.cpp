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
	this->mShaderID = glCreateShader(SHADER_TYPE);
	glShaderSource(mShaderID, 1, &SHADER_CODE, NULL);
	glCompileShader(mShaderID);

	GLint success;
	glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLength;
		glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &logLength);
		vector<GLchar> infoLog(logLength);
		glGetShaderInfoLog(mShaderID, logLength, NULL, infoLog.data());
		cerr << "Shader compilation failed: " << infoLog.data() << endl;
	}
}

bool Shader::createShaderFromFile(const GLenum SHADER_TYPE, const char* SHADER_FILE)
{
	ifstream file(SHADER_FILE);
	if (!file.is_open())
	{
		cout << "Unable to open file " << SHADER_FILE << endl;
		return false;
	}
	string shaderCode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	this->createShader(SHADER_TYPE, shaderCode.c_str());
	return true;
}

bool Shader::createShaderFromFile(const GLenum SHADER_TYPE, const string& SHADER_FILE)
{
	ifstream file(SHADER_FILE);
	if (!file.is_open())
	{
		cout << "Unable to open file " << SHADER_FILE << endl;
		return false;
	}
	string shaderCode((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	this->createShader(SHADER_TYPE, shaderCode.c_str());
	return true;
}

void Shader::attachShader(const GLuint SHADER_PROGRAM_ID) const
{
	glAttachShader(SHADER_PROGRAM_ID, mShaderID);
}

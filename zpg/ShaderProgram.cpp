#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* VERTEX_SHADER, const char* FRAGMENT_SHADER) : mVertexShader(VERTEX_SHADER), mFragmentShader(FRAGMENT_SHADER)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &mVertexShader, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &mFragmentShader, NULL);
	glCompileShader(fragmentShader);
	mID = glCreateProgram();
	glAttachShader(mID, fragmentShader);
	glAttachShader(mID, vertexShader);
	glLinkProgram(mID);

	GLint status;
	glGetProgramiv(mID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(mID, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
		exit(EXIT_FAILURE);

	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(mID); }

void ShaderProgram::use() const
{
	glUseProgram(mID);
}

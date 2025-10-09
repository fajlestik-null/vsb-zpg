#include "ShaderProgram.h"
#include "Camera.h"

/*ShaderProgram::ShaderProgram()
{
	mVertexShader = new Shader();
	mVertexShader->createShader(GL_VERTEX_SHADER, vertex_shader_def);
	mFragmentShader = new Shader();
	mFragmentShader->createShader(GL_FRAGMENT_SHADER, fragment_shader_def);

	mID = glCreateProgram();
	mVertexShader->attachShader(mID);
	mFragmentShader->attachShader(mID);
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
}*/

ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragmentShader)
{
	mVertexShader = new Shader();
	mVertexShader->createShader(GL_VERTEX_SHADER, vertexShader);
	mFragmentShader = new Shader();
	mFragmentShader->createShader(GL_FRAGMENT_SHADER, fragmentShader);

	mID = glCreateProgram();
	mVertexShader->attachShader(mID);
	mFragmentShader->attachShader(mID);
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
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix)
{

	GLint location = glGetUniformLocation(mID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform " << name << "not found in shader!" << std::endl;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vector)
{
	GLint location = glGetUniformLocation(mID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform " << name << " not found in shader!" << std::endl;
		return;
	}

	glUniform3fv(location, 1, glm::value_ptr(vector));
}

void ShaderProgram::useShader()
{
	glUseProgram(mID);
}

void ShaderProgram::useShader(glm::mat4 M)
{
	glUseProgram(mID);
	GLint idModelTransform = glGetUniformLocation(mID, "modelMatrix");
	glUniformMatrix4fv(idModelTransform, 1, GL_FALSE, &M[0][0]);
}


void ShaderProgram::onCameraChanged(Camera* camera)
{
	setUniform("viewMatrix", camera->getViewMatrix());
	setUniform("projectionMatrix", camera->getProjectionMatrix());
}

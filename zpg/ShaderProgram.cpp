#include "ShaderProgram.h"
#include "Subject.h"

ShaderProgram::ShaderProgram(ShaderLoadType type, const char* vertexShader, const char* fragmentShader)
{
	mVertexShader = new Shader();
	mFragmentShader = new Shader();

	if (type == ShaderLoadType::VARIABLE)
	{
		mVertexShader->createShader(GL_VERTEX_SHADER, vertexShader);
		mFragmentShader->createShader(GL_FRAGMENT_SHADER, fragmentShader);
	}
	else if (type == ShaderLoadType::FILE)
	{
		mVertexShader->createShaderFromFile(GL_VERTEX_SHADER, vertexShader);
		mFragmentShader->createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShader);
	}

	mID = glCreateProgram();
	mVertexShader->attachShader(mID);
	mFragmentShader->attachShader(mID);

	glLinkProgram(mID);
	this->checkLinker();
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix)
{

	GLint location = glGetUniformLocation(mID, name.c_str());
	if (location == -1)
	{
		std::cerr << "Uniform " << name << " not found in shader!" << std::endl;
		
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

void ShaderProgram::checkLinker()
{
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

void ShaderProgram::useShader()
{
	glUseProgram(mID);
}

void ShaderProgram::useShader(glm::mat4 M)
{
	glUseProgram(mID);
	this->setUniform("modelMatrix", M);
}

void ShaderProgram::notify(Subject* subject)
{		
	if (!subject) return;

	if (Camera* camera = dynamic_cast<Camera*>(subject)) {
		this->onCameraChanged(camera);
	}
	else if (Light* light = dynamic_cast<Light*>(subject)) {
		this->onLightChanged(light);
	}
}

void ShaderProgram::onCameraChanged(Camera* camera)
{
	glUseProgram(mID);
	this->setUniform("viewMatrix", camera->getViewMatrix());
	this->setUniform("projectionMatrix", camera->getProjectionMatrix());
	this->setUniform("cameraPosition", camera->getPosition());
}

void ShaderProgram::onLightChanged(Light* light)
{
	glUseProgram(mID);
	this->setUniform("lightPosition", light->getPosition());
	this->setUniform("lightColor", light->getColor());
}

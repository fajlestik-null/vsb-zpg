#include "ShaderProgram.h"
#include "Subject.h"

ShaderProgram::ShaderProgram(const ShaderLoadType LOAD_TYPE, const char* VERTEX_SHADER, const char* FRAGMENT_SHADER)
{
	this->mVertexShader = new Shader();
	this->mFragmentShader = new Shader();

	if (LOAD_TYPE == ShaderLoadType::VARIABLE)
	{
		this->mVertexShader->createShader(GL_VERTEX_SHADER, VERTEX_SHADER);
		this->mFragmentShader->createShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
	}
	else if (LOAD_TYPE == ShaderLoadType::FILE)
	{
		this->mVertexShader->createShaderFromFile(GL_VERTEX_SHADER, VERTEX_SHADER);
		this->mFragmentShader->createShaderFromFile(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
	}

	this->mID = glCreateProgram();
	this->mVertexShader->attachShader(mID);
	this->mFragmentShader->attachShader(mID);

	glLinkProgram(mID);
	this->checkLinker();
}

ShaderProgram::~ShaderProgram()
{
	delete mVertexShader;
	delete mFragmentShader;
	glDeleteProgram(mID);
}

void ShaderProgram::setUniform(const string& NAME, const mat4& MATRIX) const
{

	GLint location = glGetUniformLocation(mID, NAME.c_str());
	if (location == -1)
	{
		cerr << "Uniform " << NAME << " not found in shader!" << endl;
		
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(MATRIX));

}

void ShaderProgram::setUniform(const string& NAME, const vec3& VECTOR) const
{
	GLint location = glGetUniformLocation(mID, NAME.c_str());
	if (location == -1)
	{
		cerr << "Uniform " << NAME << " not found in shader!" << endl;
		return;
	}

	glUniform3fv(location, 1, value_ptr(VECTOR));
}

void ShaderProgram::setUniform(const string& NAME, Light *light) const
{
	if (NAME == "lightPosition")
	{
		this->setUniform("lightPosition", light->getPosition());
	}
	else if (NAME == "lightColor")
	{
		this->setUniform("lightColor", light->getColor());
	}
}

void ShaderProgram::setUniform(const string& NAME, Camera* camera) const
{
	if (NAME == "cameraPosition")
	{
		this->setUniform("cameraPosition", camera->getPosition());
	}
	else if (NAME == "projectionMatrix")
	{
		this->setUniform("projectionMatrix", camera->getProjectionMatrix());
	}
	else if (NAME == "viewMatrix")
	{
		this->setUniform("viewMatrix", camera->getViewMatrix());
	}
}

void ShaderProgram::checkLinker() const
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

void ShaderProgram::useShader() const
{
	glUseProgram(mID);
}

void ShaderProgram::useShader(const mat4 MATRIX) const
{
	glUseProgram(mID);
	this->setUniform("modelMatrix", MATRIX);
}

void ShaderProgram::notify(Subject* subject)
{
	if (!subject) return;

	glUseProgram(mID);
	if (Camera* camera = dynamic_cast<Camera*>(subject)) {
		this->setUniform("projectionMatrix", camera);
		this->setUniform("viewMatrix", camera);
		this->setUniform("cameraPosition", camera);
	}
	else if (Light* light = dynamic_cast<Light*>(subject)) {
		this->setUniform("lightPosition", light);
		this->setUniform("lightColor", light);
	}
}

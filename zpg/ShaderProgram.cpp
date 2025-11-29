#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"
#include "WorldEntity.h"
#include "DrawableObject.h"
#include "Material.h"


ShaderProgram::ShaderProgram()
{

}

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

bool ShaderProgram::loadShaderProgramFromFile(const string& VERTEX_SHADER, const string& FRAGMENT_SHADER)
{
		this->mVertexShader = new Shader();
		this->mFragmentShader = new Shader();
		if (!(this->mVertexShader->createShaderFromFile(GL_VERTEX_SHADER, VERTEX_SHADER) && this->mFragmentShader->createShaderFromFile(GL_FRAGMENT_SHADER, FRAGMENT_SHADER)))
		{
			return false;
		}


		this->mID = glCreateProgram();
		this->mVertexShader->attachShader(mID);
		this->mFragmentShader->attachShader(mID);
		glLinkProgram(mID);


		this->checkLinker();

		// cleanup
		//glDeleteShader(vertex_shader);
		//glDeleteShader(fragment_shader);


		return true;
}

void ShaderProgram::setUniform(const string& NAME, const mat4& MATRIX) const
{

	GLint location = glGetUniformLocation(mID, NAME.c_str());
	if (location == -1 && mDebugFlag)
	{
		cerr << "Uniform " << NAME << " not found in shader!" << endl;
		
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(MATRIX));

}

void ShaderProgram::setUniform(const string& NAME, const float& FLOAT) const
{

	GLint location = glGetUniformLocation(mID, NAME.c_str());
	if (location == -1 && mDebugFlag)
	{
		cerr << "Uniform " << NAME << " not found in shader!" << endl;

	}

	glUniform1f(location, FLOAT);
}

void ShaderProgram::setUniform(const string& NAME, const int& INT) const
{
	glUseProgram(mID);
	GLint location = glGetUniformLocation(mID, NAME.c_str());
	if (location == -1 && mDebugFlag)
	{
		cerr << "Uniform " << NAME << " not found in shader!" << endl;

	}

	glUniform1i(location, INT);
}

void ShaderProgram::setUniform(const string& NAME, const vec3& VECTOR) const
{
	GLint location = glGetUniformLocation(mID, NAME.c_str());
	if (location == -1 && mDebugFlag)
	{
		cerr << "Uniform " << NAME << " not found in shader!" << endl;
		return;
	}

	glUniform3fv(location, 1, value_ptr(VECTOR));
}

void ShaderProgram::setUniform(Light *light) const
{
	mat4 lightModelMatrix = light->getTransformManager()->getFinalMatrix();

    vec3 lightPosition = vec3(lightModelMatrix[3]);

	vec4 localDirection = vec4(0.0f, 0.0f, -1.0f, 0.0f); // Assuming the light points down the negative Z-axis in its local space
	vec3 lightDirection = normalize(vec3(lightModelMatrix*localDirection));
		
	this->setUniform("lights[" + to_string(light->getID()) + "].position", lightPosition);
	this->setUniform("lights[" + to_string(light->getID()) + "].color", light->getLightColor());
	this->setUniform("lights[" + to_string(light->getID()) + "].intensity", light->getIntensity());
	this->setUniform("lights[" + to_string(light->getID()) + "].direction", lightDirection);
	this->setUniform("lights[" + to_string(light->getID()) + "].type", light->getLightType());
	this->setUniform("lights[" + to_string(light->getID()) + "].distance", light->getDistance());
	this->setUniform("numLights", Light::LIGHT_COUNT);
}

void ShaderProgram::setUniform(Camera* camera) const
{
		this->setUniform("cameraPosition", camera->getPosition());
		this->setUniform("projectionMatrix", camera->getProjectionMatrix());
		this->setUniform("viewMatrix", camera->getViewMatrix());
}

void ShaderProgram::setUniform(Material* material)
{
	this->setUniform("material.ambientFactor", material->getAmbientFactor());
	this->setUniform("material.diffuseFactor", material->getDiffuseFactor());
	this->setUniform("material.specularFactor", material->getSpecularFactor());
	this->setUniform("material.shininess", material->getShininess());
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

void ShaderProgram::useShader(const mat4 MATRIX, const vec3 COLOR) const
{
	glUseProgram(mID);
	this->setUniform("modelMatrix", MATRIX);
	this->setUniform("sourceObjectColor", COLOR);
}

void ShaderProgram::turnDebugOn()
{
	mDebugFlag = true;
}

void ShaderProgram::turnDebugOff()
{
	mDebugFlag = false;
}

void ShaderProgram::notify(Subject* subject)
{
	if (!subject) return;

	glUseProgram(mID);
	if (Camera* camera = dynamic_cast<Camera*>(subject)) {
		this->setUniform(camera);
		return;
	}
	else if (Light* light = dynamic_cast<Light*>(subject)) {
		this->setUniform(light);
	}

	WorldEntity* we = dynamic_cast<WorldEntity*>(subject);

	if (we != nullptr)
	{
		Material* mat = we->getMaterial();

		if (mat != nullptr)
		{
			this->setUniform(mat);
		}
	}
}

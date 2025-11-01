#pragma once
#include "Includes.h"
#include "Shader.h"
#include "IObserver.h"
#include "ShaderLoadType.h"

class Camera;
class Light;

class ShaderProgram : public IObserver
{
private:
    GLuint mID;
    Shader* mVertexShader;
    Shader* mFragmentShader;

    void checkLinker() const;

    void setUniform(const string& NAME, const mat4& MATRIX) const;
    void setUniform(const string& NAME, const float& FLOAT) const;
    void setUniform(const string& NAME, const int& INT) const;
    void setUniform(const string& NAME, const vec3& VECTOR) const;
    void setUniform(Light* light) const;
    void setUniform( Camera* camera) const;


public:
	ShaderProgram();
    ShaderProgram(const ShaderLoadType LOAD_TYPE, const char* VERTEX_SHADER, const char* FRAGMENT_SHADER);
    ~ShaderProgram();

    bool loadShaderProgramFromFile(const string& VERTEX_SHADER, const string& FRAGMENT_SHADER);

	void notify(Subject* subject) override;

    void useShader() const;
    void useShader(const mat4 MATRIX, const vec3 COLOR) const;
};
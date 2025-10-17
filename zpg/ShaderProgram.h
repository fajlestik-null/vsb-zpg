#pragma once
#include "Includes.h"
#include "Shader.h"
#include "IObserver.h"
#include "ShaderLoadType.h"
#include "Camera.h"
#include "Light.h"

class ShaderProgram : public IObserver
{
private:
    GLuint mID;
    Shader* mVertexShader;
    Shader* mFragmentShader;

public:
    ShaderProgram(const ShaderLoadType LOAD_TYPE, const char* VERTEX_SHADER, const char* FRAGMENT_SHADER);
    ~ShaderProgram() 
    { 
		delete mVertexShader;
		delete mFragmentShader;
        glDeleteProgram(mID);
    }

	void notify(Subject* subject) override;

    void onCameraChanged(Camera* camera) const;
	void onLightChanged(Light* light) const;
    void setUniform(const string& NAME, const mat4& MATRIX) const;
    void setUniform(const string& NAME, const vec3& VECTOR) const;
    void checkLinker() const;
    void useShader() const;
    void useShader(const mat4 MATRIX) const;
};
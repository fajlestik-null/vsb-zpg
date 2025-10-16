#pragma once
#include "Includes.h"
#include "Shader.h"
#include "IObserver.h"
#include "Camera.h"
#include "Light.h"
#include "ShaderLoadType.h"

class ShaderProgram : public IObserver
{
private:
    GLuint mID;
    Shader* mVertexShader = nullptr;
    Shader* mFragmentShader = nullptr;

public:
    ShaderProgram(ShaderLoadType type, const char* vertexShader, const char* fragmentShader);
    ~ShaderProgram() { glDeleteProgram(mID); }

	void notify(ISubject* subject) override;

    void onCameraChanged(Camera* camera);
	void onLightChanged(Light* light);
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const glm::vec3& vector);
    void checkLinker();
    void useShader();
    void useShader(glm::mat4 M);
};
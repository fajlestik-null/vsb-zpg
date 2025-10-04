#pragma once
#include "Includes.h"


class ShaderProgram
{
private:
    GLuint mID;
    const char* mVertexShader;
    const char* mFragmentShader;

public:
    ShaderProgram(const char* VERTEX_SHADER, const char* FRAGMENT_SHADER);
    ~ShaderProgram() { glDeleteProgram(mID); }
    void createShader(int a) {};
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const glm::vec3& vector);
    void useShader();
    void useShader(glm::mat4 M);
};
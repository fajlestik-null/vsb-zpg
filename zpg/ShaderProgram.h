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
    ~ShaderProgram();
    void use(glm::mat4 M);
};
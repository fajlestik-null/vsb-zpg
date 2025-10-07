#pragma once
#include "Includes.h"
#include "Shader.h"

/*extern const char* vertex_shader_def =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"uniform mat4 modelMatrix;"
"void main () {"
"     gl_Position = modelMatrix * vec4 (vp, 1.0);"
"}";

extern const char* fragment_shader_def =
"#version 330\n"
"out vec4 fragColor;"
"void main () {"
"     fragColor = vec4 (0.5, 0.0, 0.5, 1.0);"
"}";*/

class ShaderProgram
{
private:
    GLuint mID;
    Shader* mVertexShader = nullptr;
    Shader* mFragmentShader = nullptr;

public:
    //ShaderProgram();
    ShaderProgram(const char* vertexShader, const char* fragmentShader);
    //ShaderProgram(std::string VertexShaderPath, std::string fragmentShaderPath);
    ~ShaderProgram() { glDeleteProgram(mID); }
    void createShader(int a) {};
    void setUniform(const std::string& name, const glm::mat4& matrix);
    void setUniform(const std::string& name, const glm::vec3& vector);
    void useShader();
    void useShader(glm::mat4 M);
};
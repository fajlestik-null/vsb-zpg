//Include GLEW
#include <GL/glew.h>
//Include GLFW
#include <GLFW/glfw3.h> 

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

#include <SOIL.h>

#include<assimp/Importer.hpp>// C++ importerinterface
#include<assimp/scene.h>// aiSceneoutputdata structure
#include<assimp/postprocess.h>// Post processingflags


#include <iostream>
#include <fstream>
#include <iomanip>   // std::setprecision, std::setw
#include <vector>

#include "ShaderLoader.h"

using namespace std;

GLuint shaderProgram=0;

static void error_callback(int error, const char* description) { 
    fputs(description, stderr); 
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    if (action == GLFW_PRESS && key>=321 && key<=329) {
        GLint idTexUnit = glGetUniformLocation(shaderProgram, "intensity");
        glUniform1i(idTexUnit, key-320);
    }
}

int main(void)
{

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }
    
    window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
    
    int count=0;
    Assimp::Importer importer;
    unsigned int importOptions = aiProcess_Triangulate
        | aiProcess_OptimizeMeshes              // sloučení malých plošek
        | aiProcess_JoinIdenticalVertices       // NUTNÉ jinak hodně duplikuje
        | aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
        | aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy
    const aiScene* scene = importer.ReadFile("model.obj", importOptions);


    vector<float> data;
    if (scene) {
        aiMesh* mesh = scene->mMeshes[0];
        count = mesh->mNumFaces * 3;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++){
            for (unsigned int j = 0; j < 3; j++)
            {
                data.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mVertices[mesh->mFaces[i].mIndices[j]].z);
                data.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mNormals[mesh->mFaces[i].mIndices[j]].z);
                data.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mTextureCoords[0][mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].x);
                data.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].y);
                data.push_back(mesh->mTangents[mesh->mFaces[i].mIndices[j]].z);
            }
        }
    }
    //Vertex Array Object (VAO)
    GLuint VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), &data[0], GL_STATIC_DRAW);
    
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //enable vertex attributes
    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)(sizeof(float) * 6));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (GLvoid*)(sizeof(float) * 8));



    //Create and compile shaders
    new ShaderLoader("PhongVertexShader.glsl", "PhongFragmentShader.glsl", &shaderProgram);
    
    //Textures - Albedo
    glActiveTexture(GL_TEXTURE0);
    GLuint image = SOIL_load_OGL_texture("albedo.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (image == NULL) {
        std::cout << "An error occurred while loading Albedo." << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, image);
    //Textures - Normal Map
    glActiveTexture(GL_TEXTURE1);
    image = SOIL_load_OGL_texture("normalmap.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (image == NULL) {
        std::cout << "An error occurred while loading NormalMap." << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, image);

    glm::mat4 M = glm::mat4(1.0f);
    float angle = 0;
    
    glUseProgram(shaderProgram);
    GLint idTexUnit = glGetUniformLocation(shaderProgram, "UIAlbedo");
    glUniform1i(idTexUnit, 0);
    idTexUnit = glGetUniformLocation(shaderProgram, "UINormal");
    glUniform1i(idTexUnit, 1);
    
    glfwSetKeyCallback(window, key_callback);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        M = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        GLint idModelTransform = glGetUniformLocation(shaderProgram, "modelMatrix");
        glUniformMatrix4fv(idModelTransform, 1, GL_FALSE, &M[0][0]);

        // draw triangles
        glDrawArrays(GL_TRIANGLES, 0, count); //mode,first,count
        glfwPollEvents();
        glfwSwapBuffers(window);
        angle += 0.01f;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
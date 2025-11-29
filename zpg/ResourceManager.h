#pragma once

#include "Includes.h"

#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"

class ResourceManager
{
private:
    unordered_map<string, Model*> mModels;
    unordered_map<string, ShaderProgram*> mShaderPrograms;
    unordered_map<string, Texture*> mTextures;

    ResourceManager();

public:
    static ResourceManager& getInstance();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    Model* loadModel(const string& PATH);

    Model* getModel(const string& PATH);

    ShaderProgram* loadShaderProgram(const string& VERTEX_SHADER, const string& FRAGMENT_SHADER);

    Texture* loadTexture(const string& PATH);

    Texture* loadCubeMap(const string& NAME, const vector<string>& FACES);

    Texture* getTexture(const string& PATH);

    void clear();
};
#include "ResourceManager.h"

ResourceManager::ResourceManager() = default;

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager manager;
    return manager;
}

Model* ResourceManager::loadModel(const string& PATH)
{
    auto it = mModels.find(PATH);
    if (it != mModels.end())
        return it->second;

    Model* model = new Model();
    if (!model->loadModelFromFile(PATH))
    {
        cerr << "Error while loading: " << PATH << endl;
        return nullptr;
    }

    mModels[PATH] = model;
    return model;
}

Model* ResourceManager::getModel(const string& PATH)
{
    auto it = mModels.find(PATH);
    return it != mModels.end() ? it->second : nullptr;
}

ShaderProgram* ResourceManager::loadShaderProgram(const string& VERTEX_SHADER, const string& FRAGMENT_SHADER)
{
    string key = VERTEX_SHADER + "|" + FRAGMENT_SHADER;

    auto it = mShaderPrograms.find(key);
    if (it != mShaderPrograms.end())
        return it->second;

    ShaderProgram* shaderProgram = new ShaderProgram();
    if (!shaderProgram->loadShaderProgramFromFile(VERTEX_SHADER, FRAGMENT_SHADER))
    {
        cerr << "Error while loading: " << key << endl;
        return nullptr;
    }

    mShaderPrograms[key] = shaderProgram;
    return shaderProgram;
}

Texture* ResourceManager::loadTexture(const string& PATH)
{
    auto it = mTextures.find(PATH);
    if (it != mTextures.end())
        return it->second;

    Texture* texture = new Texture(PATH);
    if (texture->getID() == 0)
    {
        cerr << "Failed to load texture: " << PATH << endl;
        delete texture;
        return nullptr;
    }

    mTextures[PATH] = texture;
    return texture;
}

Texture* ResourceManager::loadCubeMap(const string& NAME, const vector<string>& FACES)
{
    auto it = mTextures.find(NAME);
    if (it != mTextures.end())
        return it->second;

    Texture* cubeMap = new Texture(FACES);
    if (cubeMap->getID() == 0)
    {
        cerr << "Failed to load cubemap: " << NAME << endl;
        delete cubeMap;
        return nullptr;
    }

    mTextures[NAME] = cubeMap;
    return cubeMap;
}

Texture* ResourceManager::getTexture(const string& PATH)
{
    auto it = mTextures.find(PATH);
    return it != mTextures.end() ? it->second : nullptr;
}

void ResourceManager::clear()
{
    mModels.clear();
    mShaderPrograms.clear();
}
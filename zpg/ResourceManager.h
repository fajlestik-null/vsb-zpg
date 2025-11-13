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

    ResourceManager() = default;

public:
    static ResourceManager& getInstance()
    {
        static ResourceManager manager;
        return manager;
    }

    // Prevent copying
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    Model* loadModel(const string& PATH)
    {
        auto it = mModels.find(PATH);
        if (it != mModels.end())
            return it->second;

		Model* model = new Model();
        if (!model->loadModelFromFile(PATH))
        {
            std::cerr << "Error while loading: " << PATH << std::endl;
            return nullptr;
        }

        mModels[PATH] = model;
        return model;
    }

    Model* getModel(const string& PATH)
    {
        auto it = mModels.find(PATH);
        return it != mModels.end() ? it->second : nullptr;
    }

    ShaderProgram* loadShaderProgram(const string& VERTEX_SHADER, const string& FRAGMENT_SHADER)
    {
        string key = VERTEX_SHADER + "|" + FRAGMENT_SHADER;

        auto it = mShaderPrograms.find(key);
        if (it != mShaderPrograms.end())
            return it->second;

        ShaderProgram* shaderProgram = new ShaderProgram();
        if (!shaderProgram->loadShaderProgramFromFile(VERTEX_SHADER,FRAGMENT_SHADER))
        {
            std::cerr << "Error while loading: " << key << std::endl;
            return nullptr;
        }

        mShaderPrograms[key] = shaderProgram;
        return shaderProgram;
    }

    Texture* loadTexture(const std::string& PATH)
    {
        // Check if texture is already loaded
        auto it = mTextures.find(PATH);
        if (it != mTextures.end())
            return it->second;

        // Load the texture; Texture class handles units automatically
        Texture* texture = new Texture(PATH);
        if (texture->getID() == 0)
        {
            std::cerr << "Failed to load texture: " << PATH << std::endl;
            delete texture;
            return nullptr;
        }

        // Store it for reuse
        mTextures[PATH] = texture;
        return texture;
    }

    Texture* loadCubeMap(const std::string& NAME, const std::vector<std::string>& FACES)
    {
        // Check if already loaded
        auto it = mTextures.find(NAME);
        if (it != mTextures.end())
            return it->second;

        // Create cubemap texture
        Texture* cubeMap = new Texture(FACES);
        if (cubeMap->getID() == 0)
        {
            std::cerr << "Failed to load cubemap: " << NAME << std::endl;
            delete cubeMap;
            return nullptr;
        }

        mTextures[NAME] = cubeMap;
        return cubeMap;
    }

    Texture* getTexture(const std::string& PATH)
    {
        auto it = mTextures.find(PATH);
        return it != mTextures.end() ? it->second : nullptr;
    }


    void clear()
    {
        mModels.clear();
        mShaderPrograms.clear();
    }
};
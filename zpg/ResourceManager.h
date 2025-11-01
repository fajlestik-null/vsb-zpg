#pragma once

#include "Includes.h"

#include "Model.h"
#include "ShaderProgram.h"


class ResourceManager
{
private:
    unordered_map<string, Model*> mModels;
    unordered_map<string, ShaderProgram*> mShaderPrograms;

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

    void clear()
    {
        mModels.clear();
        mShaderPrograms.clear();
    }
};
#pragma once
#include "Subject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "TransformManager.h"
#include "Controls.h"

#include "Includes.h"

class WorldEntity : public Subject
{
protected:
	bool mVisible = true;

	vec3 mObjectColor = vec3(1.0f, 1.0f, 1.0f);

	Model* mModel;
	vector<ShaderProgram*> mShaderPrograms;
    shared_ptr<TransformManager> mTransformManager;

public:
    WorldEntity(): mModel(nullptr), mTransformManager(make_shared<TransformManager>()), mObjectColor(vec3(1.0f, 1.0f, 1.0f)) {}

    virtual ~WorldEntity() = default;

    void draw()
    {
        if (!mModel || !mVisible || mShaderPrograms.empty())
            return;

        for (auto& shaderProgram : mShaderPrograms)
        {
            shaderProgram->useShader(mTransformManager->getFinalMatrix(), mObjectColor);

            if (mModel->getTexture())
            {
                shaderProgram->setUniform("textureUnitID", mModel->getTexture()->getUnitIndex());
                mModel->getTexture()->bind(); // make sure the correct texture is active
            }
        }
            mModel->put();
    }

    virtual void update(GLFWwindow* window, float deltaTime, Controls* controls) {
		notifyObservers();
    };

    Model* getModel() const { return mModel; }
    vector<ShaderProgram*> getShaderProgram() const { return mShaderPrograms; }
    shared_ptr<TransformManager> getTransformManager() const { return mTransformManager; }
    bool isVisible() const { return mVisible; }
    vec3 getColor() const { return mObjectColor; }
    
    void setModel(Model* model) { mModel = model; }
    void addShaderProgram(ShaderProgram* shaderProgram) { mShaderPrograms.push_back(shaderProgram); }
    void setTransformManager(shared_ptr<TransformManager> transformManager) { mTransformManager = move(transformManager); }
    void setToVisible(const bool VISIBLE) { mVisible = VISIBLE; }
    void setColor(const vec3 COLOR) { mObjectColor = COLOR; }

    SubjectType getType() const override { return SubjectType::WORLD_ENTITY; }

    void addStaticTransform(TransformBase* transformation)
    {
        mTransformManager->addStaticTransform(transformation);
    }
    void addLocalTransform(TransformBase* transformation)
    {
        mTransformManager->addLocalTransform(transformation);
    }
    void addGlobalTransform(TransformBase* transformation)
    {
        mTransformManager->addGlobalTransform(transformation);
    }

    void addParent(std::shared_ptr<TransformManager> parent)
    {
        mTransformManager->addParent(parent);
    }
};

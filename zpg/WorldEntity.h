#pragma once
#include "Subject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "TransformManager.h"
#include "Controls.h"
#include "Texture.h"

#include "Includes.h"

class WorldEntity : public Subject
{
protected:
	//in progress
    GLuint mStencilIndex = 0;
	bool mVisible = true;
	vec3 mObjectColor = vec3(1.0f, 1.0f, 1.0f);

	Model* mModel;
	vector<ShaderProgram*> mShaderPrograms;
    shared_ptr<TransformManager> mTransformManager;
    Texture* mTexture;


public:
    WorldEntity(): mModel(nullptr), mTransformManager(make_shared<TransformManager>()), mObjectColor(vec3(1.0f, 1.0f, 1.0f)), mTexture(nullptr), mStencilIndex(0){}

    virtual ~WorldEntity() = default;

    void draw()
    {
        if (!mModel || !mVisible || mShaderPrograms.empty())
            return;

        for (auto& shaderProgram : mShaderPrograms)
        {
            shaderProgram->useShader(mTransformManager->getFinalMatrix(), mObjectColor);

            if (mTexture != nullptr)
            {
                shaderProgram->setUniform("textureUnitID", mTexture->getUnitIndex());
				shaderProgram->setUniform("hasTexture", 1);
                mTexture->bind();
            }
            else
            {
                shaderProgram->setUniform("hasTexture", 0);
            }
        }

        mModel->put();

            // Optional: Unbind the texture after drawing
            if (mTexture != nullptr)
            {
                mTexture->unbind();
            }
    }

    virtual void update(GLFWwindow* window, float deltaTime, Controls* controls) {
		notifyObservers();
    };

    WorldEntity * getCopy() const
    {
        WorldEntity * duplicate = new WorldEntity();

        duplicate->setStencilIndex(this->getStencilIndex()); // may be removed based on usage of Stencil buffer index

        duplicate->setToVisible(this->isVisible()); //currently not used

        duplicate->setColor(this->getColor());

		duplicate->setModel(this->getModel());
		duplicate->setTexture(this->getTexture());
		duplicate->addShaderProgram(this->getShaderProgram()[0]); // only first shader program is copied - I use only one shader per entity currently anyway

        duplicate->setTransformManager(std::make_shared<TransformManager>());

        return duplicate;
    }

    Model* getModel() const { return mModel; }
    vector<ShaderProgram*> getShaderProgram() const { return mShaderPrograms; }
    shared_ptr<TransformManager> getTransformManager() const { return mTransformManager; }
    bool isVisible() const { return mVisible; }
    vec3 getColor() const { return mObjectColor; }
    Texture* getTexture() const { return mTexture; }
    GLuint getStencilIndex() const { return mStencilIndex; }
    
    void setTexture(Texture* texture) { mTexture = texture; }
    void setModel(Model* model) { mModel = model; }
    void addShaderProgram(ShaderProgram* shaderProgram) { mShaderPrograms.push_back(shaderProgram); }
    void setTransformManager(shared_ptr<TransformManager> transformManager) { mTransformManager = move(transformManager); }
    void setToVisible(const bool VISIBLE) { mVisible = VISIBLE; }
    void setColor(const vec3 COLOR) { mObjectColor = COLOR; }
	void setStencilIndex(const int INDEX) { mStencilIndex = INDEX; }

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

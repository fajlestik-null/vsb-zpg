#pragma once
#include "Subject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "TransformManager.h"
#include "Controls.h"
#include "Texture.h"
#include "Material.h"

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
	Material* mMaterial;
    Texture* mTexture;
    //need to add material

    static GLuint sNextIndex;

public:
    WorldEntity(): mModel(nullptr), mTransformManager(make_shared<TransformManager>()), mObjectColor(vec3(1.0f, 1.0f, 1.0f)), mTexture(nullptr), mStencilIndex(0), mMaterial(new Material()){}

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
				shaderProgram->setUniform(mMaterial);
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
		mTransformManager->calculateTransform();
		notifyObservers();
    };

    WorldEntity * getCopy() const
    {
        if (sNextIndex++ > 255)
        {
            sNextIndex = 1;
        }

        WorldEntity * duplicate = new WorldEntity();

        duplicate->mStencilIndex = sNextIndex;

        duplicate->mVisible = this->mVisible; //currently not used

		duplicate->mObjectColor = this->mObjectColor;

		duplicate->mModel = this->mModel;
		duplicate->mTexture = this->mTexture;

        duplicate->mMaterial = new Material(
            this->mMaterial->getAmbientFactor(),
            this->mMaterial->getDiffuseFactor(),
            this->mMaterial->getSpecularFactor(),
            this->mMaterial->getShininess()
        );

        duplicate->mShaderPrograms = this->mShaderPrograms;
		
        duplicate->mObservers = this->mObservers;

		duplicate->mTransformManager = this->mTransformManager->getCopy();
        
        return duplicate;
    }

    Model* getModel() const { return mModel; }
    vector<ShaderProgram*> getShaderProgram() const { return mShaderPrograms; }
    shared_ptr<TransformManager> getTransformManager() const { return mTransformManager; }
    bool isVisible() const { return mVisible; }
    vec3 getColor() const { return mObjectColor; }
    Texture* getTexture() const { return mTexture; }
    GLuint getStencilIndex() const { return mStencilIndex; }
	Material* getMaterial() const { return mMaterial; }
    
    void setTexture(Texture* texture) { mTexture = texture; }
    void setModel(Model* model) { mModel = model; }
    void addShaderProgram(ShaderProgram* shaderProgram) { mShaderPrograms.push_back(shaderProgram); }
    void setTransformManager(shared_ptr<TransformManager> transformManager) { mTransformManager = move(transformManager); }
    void setToVisible(const bool VISIBLE) { mVisible = VISIBLE; }
    void setColor(const vec3 COLOR) { mObjectColor = COLOR; }
	void setStencilIndex(const int INDEX) { mStencilIndex = INDEX; }
	void setNextStencilIndex(const int INDEX) { sNextIndex = INDEX; }
	void setMaterial(Material* material) { mMaterial = material; }

    SubjectType getType() const override { return SubjectType::WORLD_ENTITY; }

    void nullifyTranslation()
    {
        // 1. Get the current overall final position (translation components from the 4th column)
        glm::mat4 finalMatrix = mTransformManager->getFinalMatrix();
        glm::vec3 currentPosition = glm::vec3(finalMatrix[3]); // This is the world position (X, Y, Z)

        // 2. Add a new static translation that is the inverse of the current position.
        // This moves the object back to the world origin (0, 0, 0).
        glm::vec3 negation = -currentPosition;

        // 3. Apply the negating translation.
        this->addStaticTransform(new Translation(negation));
    }

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

#include "WorldEntity.h"

GLuint WorldEntity::sNextIndex = 1;

WorldEntity::WorldEntity() : mModel(nullptr), mTransformManager(make_shared<TransformManager>()), mObjectColor(vec3(1.0f, 1.0f, 1.0f)), mTexture(nullptr), mNormalMap(nullptr), mStencilIndex(0), mMaterial(new Material()) {}

void WorldEntity::draw()
{
    if (!mModel || !mVisible || mShaderPrograms.empty())
        return;

    for (auto& shaderProgram : mShaderPrograms)
    {
        shaderProgram->useShader(mTransformManager->getFinalMatrix(), mObjectColor);
        shaderProgram->setUniform(mMaterial);


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

        if (mNormalMap != nullptr)
        {
            shaderProgram->setUniform("normalMapUnitID", mNormalMap->getUnitIndex());
            shaderProgram->setUniform("hasNormalMap", 1);
            mTexture->bind();
        }
        else
        {
            shaderProgram->setUniform("hasNormalMap", 0);
        }
    }

    mModel->put();

    if (mTexture != nullptr)
    {
        mTexture->unbind();
    }
    if (mNormalMap != nullptr)
    {
        mNormalMap->unbind();
    }
}

void WorldEntity::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
    mTransformManager->calculateTransform();
    notifyObservers();
}

WorldEntity* WorldEntity::getCopy() const
{
    if (sNextIndex++ > 255)
    {
        sNextIndex = 1;
    }

    WorldEntity* duplicate = new WorldEntity();

    duplicate->mStencilIndex = sNextIndex;

    duplicate->mVisible = this->mVisible;

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

Model* WorldEntity::getModel() const { return mModel; }
vector<ShaderProgram*> WorldEntity::getShaderProgram() const { return mShaderPrograms; }
shared_ptr<TransformManager> WorldEntity::getTransformManager() const { return mTransformManager; }
bool WorldEntity::isVisible() const { return mVisible; }
vec3 WorldEntity::getColor() const { return mObjectColor; }
Texture* WorldEntity::getTexture() const { return mTexture; }
Texture* WorldEntity::getNormalMap() const { return mNormalMap; }
GLuint WorldEntity::getStencilIndex() const { return mStencilIndex; }
Material* WorldEntity::getMaterial() const { return mMaterial; }

void WorldEntity::setTexture(Texture* texture) { mTexture = texture; }
void WorldEntity::setNormalMap(Texture* texture) { mNormalMap = texture; }
void WorldEntity::setModel(Model* model) { mModel = model; }
void WorldEntity::addShaderProgram(ShaderProgram* shaderProgram) { mShaderPrograms.push_back(shaderProgram); }
void WorldEntity::setTransformManager(shared_ptr<TransformManager> transformManager) { mTransformManager = transformManager; }
void WorldEntity::setToVisible(const bool VISIBLE) { mVisible = VISIBLE; }
void WorldEntity::setColor(const vec3 COLOR) { mObjectColor = COLOR; }
void WorldEntity::setStencilIndex(const int INDEX) { mStencilIndex = INDEX; }
void WorldEntity::setNextStencilIndex(const int INDEX) { sNextIndex = INDEX; }
void WorldEntity::setMaterial(Material* material) { mMaterial = material; }

SubjectType WorldEntity::getType() const { return SubjectType::WORLD_ENTITY; }

void WorldEntity::nullifyTranslation()
{
    mat4 finalMatrix = mTransformManager->getFinalMatrix();
    vec3 currentPosition = vec3(finalMatrix[3]);

    vec3 negation = -currentPosition;

    this->addStaticTransform(new Translation(negation));
}

void WorldEntity::addStaticTransform(TransformBase* transformation)
{
    mTransformManager->addStaticTransform(transformation);
}

void WorldEntity::addLocalTransform(TransformBase* transformation)
{
    mTransformManager->addLocalTransform(transformation);
}

void WorldEntity::addGlobalTransform(TransformBase* transformation)
{
    mTransformManager->addGlobalTransform(transformation);
}

void WorldEntity::addParent(shared_ptr<TransformManager> parent)
{
    mTransformManager->addParent(parent);
}
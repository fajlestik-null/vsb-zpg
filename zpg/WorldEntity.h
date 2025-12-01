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
    GLuint mStencilIndex = 0;
    bool mVisible = true;
    vec3 mObjectColor = vec3(1.0f, 1.0f, 1.0f);

    Model* mModel;
    vector<ShaderProgram*> mShaderPrograms;
    shared_ptr<TransformManager> mTransformManager;
    Material* mMaterial;
    Texture* mTexture;
	Texture* mNormalMap;

    static GLuint sNextIndex;

public:
    WorldEntity();

    virtual ~WorldEntity() = default;

    void draw();

    virtual void update(GLFWwindow* window, float deltaTime, Controls* controls);

    WorldEntity* getCopy() const;

    Model* getModel() const;
    vector<ShaderProgram*> getShaderProgram() const;
    shared_ptr<TransformManager> getTransformManager() const;
    bool isVisible() const;
    vec3 getColor() const;
    Texture* getTexture() const;
    Texture* getNormalMap() const;
    GLuint getStencilIndex() const;
    Material* getMaterial() const;

    void setTexture(Texture* texture);
    void setNormalMap(Texture* texture);
    void setModel(Model* model);
    void addShaderProgram(ShaderProgram* shaderProgram);
    void setTransformManager(shared_ptr<TransformManager> transformManager);
    void setToVisible(const bool VISIBLE);
    void setColor(const vec3 COLOR);
    void setStencilIndex(const int INDEX);
    void setNextStencilIndex(const int INDEX);
    void setMaterial(Material* material);

    SubjectType getType() const override;

    void nullifyTranslation();

    void addStaticTransform(TransformBase* transformation);
    void addLocalTransform(TransformBase* transformation);
    void addGlobalTransform(TransformBase* transformation);

    void addParent(shared_ptr<TransformManager> parent);
};
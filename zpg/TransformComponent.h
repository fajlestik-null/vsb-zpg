#pragma once

#include "Includes.h"
#include "TransformBase.h"

//Child class for specific transformation
class TransformComponent : public TransformBase {
private:
    vector<TransformBase*> mTransforms;

public:
    TransformComponent(vector<TransformBase*> transforms);

    void addTransform(TransformBase* transform);

    mat4 getModelMatrix() override;
};
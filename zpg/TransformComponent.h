#pragma once

#include "Includes.h"
#include "TransformBase.h"

class TransformComponent : public TransformBase {
private:
    vector<TransformBase*> mTransforms;

public:
    TransformComponent(vector<TransformBase*> transforms);

    void addTransform(TransformBase* transform);

    mat4 getModelMatrix() override;

	TransformBase* getCopy() const override;
};
#include "TransformComponent.h"

TransformComponent::TransformComponent(vector<TransformBase*> transforms = {}) : mTransforms(transforms) {}

void TransformComponent::addTransform(TransformBase* transform)
{
    mTransforms.push_back(transform);
}

mat4 TransformComponent::getModelMatrix() const

{
    mat4 resultMatrix = mat4(1.0f);
    for (auto transform : mTransforms) {
        resultMatrix *= transform->getModelMatrix();
    }
    return resultMatrix;
}



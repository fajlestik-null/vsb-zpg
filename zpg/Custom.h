#pragma once
#include "Includes.h"
#include "TransformBase.h"

class Custom : public TransformBase {
private:
    mat4 mMatrix;

public:

    Custom(const mat4& mat);

    mat4 getModelMatrix() override;

    TransformBase* getCopy() const override;
};
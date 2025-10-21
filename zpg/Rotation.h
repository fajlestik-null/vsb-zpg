#pragma once
#include "Includes.h"
#include "TransformBase.h"

//Child class for specific transformation
class Rotation : public TransformBase {
protected:
    vec3 mVector;
public:
    Rotation(const vec3& angles);

    mat4 getModelMatrix() const override;
};
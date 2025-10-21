#pragma once
#include "Includes.h"
#include "TransformBase.h"

//Child class for specific transformation
class Translation : public TransformBase {
private:
    vec3 mVector;
public:
    Translation(const vec3& vec);

    mat4 getModelMatrix() const override;
};

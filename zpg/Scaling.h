#pragma once
#include "Includes.h"
#include "TransformBase.h"

//Child class for specific transformation
class Scaling : public TransformBase {
private:
    vec3 mVector;
public:

    //x,z,y
    Scaling(const vec3& vec);

    mat4 getModelMatrix() const override;
};
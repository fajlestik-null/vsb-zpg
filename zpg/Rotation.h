#pragma once
#include "Includes.h"
#include "TransformBase.h"

//Child class for specific transformation
class Rotation : public TransformBase {
protected:
    vec3 mVector;
    vec3 mRandomMin;
    vec3 mRandomMax;
    bool mIsRandom = false;
    
public:
    Rotation(const vec3& angles);

    Rotation(vec3 min_range, vec3 max_range);
    
    void randomize();

    mat4 getModelMatrix() override;
};
#pragma once
#include "Includes.h"
#include "TransformBase.h"

class Rotation : public TransformBase {
protected:
    float mAngle;
    vec3 mVector;
    vec3 mRandomMin;
    vec3 mRandomMax;
    bool mIsRandom = false;
    
public:
    Rotation(const vec3& AXIS, const float ANGLE = 0.0f);

    Rotation(vec3 min_range, vec3 max_range);
    
    void randomize();

    mat4 getModelMatrix() override;

	TransformBase* getCopy() const override;
};
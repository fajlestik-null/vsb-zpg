#pragma once
#include "Includes.h"
#include "IncludeTransformations.h"

class ParametricLineMovement : public TransformBase
{
private:
    vector<vec3> mPoints;
    float mSpeedT;
    float mCurrentT = 0.0f;

    float last_time = 0.0f;

    vec3 mlastPosition;
    bool mfirstUpdate = true;

public:
    ParametricLineMovement(const vector<vec3>& POINTS,const float SPEED);

    mat4 getModelMatrix() override;


    TransformBase* getCopy() const override;
};
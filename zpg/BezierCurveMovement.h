#pragma once
#include "Includes.h"
#include "TransformBase.h"

class BezierCurveMovement : public TransformBase
{
private:
    vector<vec3> mPoints;
    float mSpeedT;
    float mCurrentT = 0.0f;

    float mLastTime = 0.0f;
    bool mfirstUpdate = true;

    mat4 mLastMatrix = mat4(1.0f);

public:
    BezierCurveMovement(const vector<vec3>& POINTS, const float SPEED);

    mat4 getModelMatrix() override;
    TransformBase* getCopy() const override;

};
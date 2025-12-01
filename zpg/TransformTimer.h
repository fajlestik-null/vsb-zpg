#pragma once
#include "Includes.h"
#include "TransformBase.h"

class TransformTimer : public TransformBase
{
private:
    float mDeltaTime;
    float mCurrentTime;
    float mLastTime;

    //{INTERVAL_WHEN_TO_APLY,TRANSFORMATION}, TIMER==INTERVAL->RESET
    vector<pair<pair<float, TransformBase *>, float>> mTransformTimers;

public:
    TransformTimer();
    TransformTimer(vector<pair<float, TransformBase*>> mTransformTimers = { {0, nullptr} });

    void addTransformation(float timer, TransformBase* transformation);

    mat4 getModelMatrix() override;

	TransformBase* getCopy() const override;
};
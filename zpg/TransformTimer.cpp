#include "TransformTimer.h"

TransformTimer::TransformTimer() : mDeltaTime(0), mCurrentTime(0), mLastTime(0)
{

}

TransformTimer::TransformTimer(vector<pair<float, TransformBase*>> mTransformTimers) : TransformBase()
{
    for (const auto& tt : mTransformTimers)
    {
        this->mTransformTimers.push_back({ tt, 0 });
    }
}

void TransformTimer::addTransformation(float timer, TransformBase* transformation)
{
    mTransformTimers.push_back({ {timer, transformation}, 0 });
}


mat4 TransformTimer::getModelMatrix() 
{
    mat4 timerMatrix(1.0f);
    mCurrentTime = (float)glfwGetTime();
    mDeltaTime = mCurrentTime - mLastTime;
    mLastTime = mCurrentTime;

    for (auto &transformTimer : mTransformTimers)
    {

        if (transformTimer.second >= transformTimer.first.first)
        {

            timerMatrix *= transformTimer.first.second->getModelMatrix();

            transformTimer.second = 0;
        }
        transformTimer.second += mDeltaTime;
    }
    return timerMatrix;
}

TransformBase* TransformTimer::getCopy() const
{
	return new TransformTimer(*this);
}

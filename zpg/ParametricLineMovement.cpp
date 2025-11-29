#include "ParametricLineMovement.h"

ParametricLineMovement::ParametricLineMovement(const vector<vec3>& POINTS,const float SPEED) : mPoints(POINTS), mSpeedT(SPEED)
{
    if (mPoints.size() < 2)
    {
        mPoints = { vec3(0.0f), vec3(1.0f) };
    }

    mlastPosition = mPoints[0];
}

mat4 ParametricLineMovement::getModelMatrix()
{
    float currentTime = (float)glfwGetTime();

    if (mfirstUpdate)
    {
        mLastTime = currentTime;

        mfirstUpdate = false;
        return mat4(1.0f);
    }

    float deltaTime = currentTime - mLastTime;
    mLastTime = currentTime;

    if (deltaTime < 0.0f) deltaTime = 0.0f;

    mCurrentT += mSpeedT * deltaTime;

    if (mCurrentT > 1.0f)
    {
        mCurrentT = fmod(mCurrentT, 1.0f);
    }

    size_t numSegments = mPoints.size() - 1;
    float tScaled = mCurrentT * (float)numSegments;
    size_t segmentIndex = (size_t)floor(tScaled);
    float u = tScaled - (float)segmentIndex;

    if (segmentIndex >= numSegments)
    {
        segmentIndex = numSegments - 1;
        u = 1.0f;
    }

    vec3 pi = mPoints[segmentIndex];
    vec3 piIncremented = mPoints[segmentIndex + 1];

    vec3 currentPosition = mix(pi, piIncremented, u);


    vec3 deltaMove = currentPosition - mlastPosition;

    mlastPosition = currentPosition;

    return translate(mat4(1.0f), deltaMove);
}

TransformBase* ParametricLineMovement::getCopy() const
{
    auto* copy = new ParametricLineMovement(*this);

    copy->mfirstUpdate = true;

    return copy;
}

#include "BezierCurveMovement.h"

BezierCurveMovement::BezierCurveMovement(const vector<vec3>& POINTS, const float SPEED)
    : mPoints(POINTS), mSpeedT(SPEED)
{
    if (mPoints.size() < 4 || (mPoints.size() - 1) % 3 != 0)
    {
        mPoints = {
            vec3(0.0f), vec3(0.0f, 1.0f, 0.0f),
            vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)
        };
    }

    mLastMatrix = mat4(1.0f);
    mLastTime = (float)glfwGetTime();
}

mat4 BezierCurveMovement::getModelMatrix()
{
    float currentTime = (float)glfwGetTime();

    float deltaTime = currentTime - mLastTime;
    mLastTime = currentTime;
    if (deltaTime < 0.0f) deltaTime = 0.0f;

    mCurrentT += mSpeedT * deltaTime;

    if (mCurrentT > 1.0f)
    {
        mCurrentT = fmod(mCurrentT, 1.0f);
    }

    size_t numSegments = (mPoints.size() - 1) / 3;
    float tScaled = mCurrentT * (float)numSegments;
    size_t segmentIndex = (size_t)floor(tScaled);
    float t = tScaled - (float)segmentIndex;

    if (segmentIndex >= numSegments)
    {
        segmentIndex = numSegments - 1;
        t = 1.0f;
    }

    size_t baseIndex = segmentIndex * 3;
    vec3 p0 = mPoints[baseIndex];
    vec3 p1 = mPoints[baseIndex + 1];
    vec3 p2 = mPoints[baseIndex + 2];
    vec3 p3 = mPoints[baseIndex + 3];

    float u = 1.0f - t;
    float u2 = u * u;
    float u3 = u2 * u;

    float t2 = t * t;
    float t3 = t2 * t;

    vec3 currentPosition =
        (u3) * p0
        + (3.0f * u2 * t) * p1
        + (3.0f * u * t2) * p2
        + (t3 * p3);



    vec3 tangent =
        3.0f * u2 * (p1 - p0)
        + 6.0f * u * t * (p2 - p1)
        + 3.0f * t2 * (p3 - p2);

    if (length(tangent) < 0.001f) {
        tangent = vec3(0.0f, 0.0f, 1.0f);
    }
    else {
        tangent = normalize(tangent);
    }

    mat4 rotation = inverse(lookAt(vec3(0.0f), -tangent, vec3(0.0f, 1.0f, 0.0f)));

    mat4 currentMatrix = translate(mat4(1.0f), currentPosition) * rotation;

    mat4 deltaMatrix;

    if (mfirstUpdate) {
        deltaMatrix = currentMatrix;
        mfirstUpdate = false;
    }
    else {
        deltaMatrix = inverse(mLastMatrix) * currentMatrix;
    }

    mLastMatrix = currentMatrix;

    return deltaMatrix;
}

TransformBase* BezierCurveMovement::getCopy() const
{
    return new BezierCurveMovement(this->mPoints,this->mSpeedT);
}
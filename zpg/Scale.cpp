#include "Scale.h"

Scale::Scale(const vec3& vec = vec3(0.0f)) : mVector(vec) {}

Scale::Scale(vec3 minRange, vec3 maxRange)
: mVector(vec3(0.0f)), mRandomMin(minRange), mRandomMax(maxRange), mIsRandom(true) {}


mat4 Scale::getModelMatrix()
{
    randomize();
    return scale(mat4(1.0f), mVector);
}

TransformBase* Scale::getCopy() const
{
	return new Scale(*this);
}

void Scale::randomize()
{
        if (mIsRandom)
        {
                mVector.x = mRandomMin.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.x - mRandomMin.x)));
                mVector.y = mRandomMin.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.y - mRandomMin.y)));
                mVector.z = mRandomMin.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.z - mRandomMin.z)));
        }
}




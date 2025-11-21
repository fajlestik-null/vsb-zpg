#include "Translation.h"

Translation::Translation(const vec3& vec = vec3(0.0f)) : mVector(vec) {}

Translation::Translation(vec3 minRange, vec3 maxRange)
: mVector(vec3(0.0f)), mRandomMin(minRange), mRandomMax(maxRange), mIsRandom(true) {}


mat4 Translation::getModelMatrix()
{
    randomize();
    return translate(mat4(1.0f), mVector);
}

TransformBase* Translation::getCopy() const
{
	return new Translation(*this);
}

void Translation::randomize()
{
        if (mIsRandom)
        {
                mVector.x = mRandomMin.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.x - mRandomMin.x)));
                mVector.y = mRandomMin.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.y - mRandomMin.y)));
                mVector.z = mRandomMin.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.z - mRandomMin.z)));
        }
}

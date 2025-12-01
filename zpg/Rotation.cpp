#include "Rotation.h"

Rotation::Rotation(const vec3& AXIS = vec3(0.0f), const float ANGLE) : mVector(AXIS), mAngle(ANGLE) {}

Rotation::Rotation(vec3 minRange, vec3 maxRange)
: mVector(vec3(0.0f)), mRandomMin(minRange), mRandomMax(maxRange), mIsRandom(true), mAngle(0.0f) {}

mat4 Rotation::getModelMatrix()
{
        mat4 rot = mat4(1.0f);
        
        randomize();

        if (mAngle == 0.0f)
        {
            // EULER
            rot = rotate(rot, radians(mVector.x), vec3(1, 0, 0));
            rot = rotate(rot, radians(mVector.y), vec3(0, 1, 0));
            rot = rotate(rot, radians(mVector.z), vec3(0, 0, 1));
        }
        else
        {
            //AXIS
            rot = rotate(rot, mAngle, mVector);
        }

        return rot;
}

TransformBase* Rotation::getCopy() const
{
	return new Rotation(*this);
}


void Rotation::randomize()
{
        if (mIsRandom)
        {
                mVector.x = mRandomMin.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.x - mRandomMin.x)));
                mVector.y = mRandomMin.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.y - mRandomMin.y)));
                mVector.z = mRandomMin.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mRandomMax.z - mRandomMin.z)));
        }
}




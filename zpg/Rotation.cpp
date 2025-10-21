#include "Rotation.h"

Rotation::Rotation(const vec3& angles = vec3(0.0f)) : mVector(angles) {}

mat4 Rotation::getModelMatrix() const
{
        mat4 rot = mat4(1.0f);
        rot = rotate(rot, radians(mVector.x), vec3(1, 0, 0));
        rot = rotate(rot, radians(mVector.y), vec3(0, 1, 0));
        rot = rotate(rot, radians(mVector.z), vec3(0, 0, 1));
        return rot;
}



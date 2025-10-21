#include "Scaling.h"

Scaling::Scaling(const vec3& vec = vec3(0.0f)) : mVector(vec) {}

mat4 Scaling::getModelMatrix() const
{
    return scale(mat4(1.0f), mVector);
}



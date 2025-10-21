#include "Translation.h"

Translation::Translation(const vec3& vec = vec3(0.0f)) : mVector(vec) {}

mat4 Translation::getModelMatrix() const
{
    return translate(mat4(1.0f), mVector);
}

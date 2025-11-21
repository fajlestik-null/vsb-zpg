#pragma once
#include "Includes.h"


// Base class for transformations - (use as instance DT)
class TransformBase {
public:
    virtual ~TransformBase() = default;
    virtual mat4 getModelMatrix() = 0;

	virtual TransformBase* getCopy() const = 0;
};

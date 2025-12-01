#pragma once
#include "Includes.h"

class TransformBase {
public:
    virtual ~TransformBase() = default;
    //x,z,y
    virtual mat4 getModelMatrix() = 0;

	virtual TransformBase* getCopy() const = 0;
};

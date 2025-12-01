#pragma once
#include "Includes.h"
#include "TransformBase.h"

class Translation : public TransformBase {
private:
    vec3 mVector;
    vec3 mRandomMin;
    vec3 mRandomMax;
    bool mIsRandom = false;
public:
    Translation(const vec3& vec);

    Translation(vec3 min_range, vec3 max_range);

    void randomize();

    mat4 getModelMatrix() override;

	TransformBase* getCopy() const override;
};

#include "ParametricLineMovement.h"

ParametricLineMovement::ParametricLineMovement(const vector<vec3>& POINTS,const float SPEED) : mPoints(POINTS), mSpeedT(SPEED)
{
    if (mPoints.size() < 2)
    {
        mPoints = { vec3(0.0f), vec3(1.0f) };
    }

    mlastPosition = mPoints[0];
}

mat4 ParametricLineMovement::getModelMatrix()
{
    float current_time = (float)glfwGetTime();

    if (mfirstUpdate)
    {
        last_time = current_time;

        mfirstUpdate = false;
        return mat4(1.0f);
    }

    float delta_time = current_time - last_time;
    last_time = current_time;

    if (delta_time < 0.0f) delta_time = 0.0f;

    mCurrentT += mSpeedT * delta_time;

    if (mCurrentT > 1.0f)
    {
        mCurrentT = fmod(mCurrentT, 1.0f);
    }

    size_t num_segments = mPoints.size() - 1;
    float t_scaled = mCurrentT * (float)num_segments;
    size_t segment_index = (size_t)floor(t_scaled);
    float u = t_scaled - (float)segment_index;

    if (segment_index >= num_segments)
    {
        segment_index = num_segments - 1;
        u = 1.0f;
    }

    vec3 P_i = mPoints[segment_index];
    vec3 P_i_plus_1 = mPoints[segment_index + 1];

    vec3 current_position = mix(P_i, P_i_plus_1, u);


    vec3 delta_move = current_position - mlastPosition;

    mlastPosition = current_position;

    return translate(mat4(1.0f), delta_move);
}

TransformBase* ParametricLineMovement::getCopy() const
{
    auto* copy = new ParametricLineMovement(*this);

    copy->mfirstUpdate = true;

    return copy;
}

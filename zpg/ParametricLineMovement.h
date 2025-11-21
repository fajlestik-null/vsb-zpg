#pragma once
#include "Includes.h"
#include "IncludeTransformations.h"

class ParametricLineMovement : public TransformBase
{
private:
    vector<vec3> mPoints;
    float mSpeedT;
    float mCurrentT = 0.0f; // Initialize to 0

    // Time tracking
    float last_time = 0.0f;

    // Delta tracking
    vec3 mlastPosition;
    bool mfirstUpdate = true;

public:
    ParametricLineMovement(const vector<vec3>& pts, float speed)
        : mPoints(pts), mSpeedT(speed)
    {
        if (mPoints.size() < 2)
        {
            mPoints = { vec3(0.0f), vec3(1.0f) };
        }
        // Initialize mlastPosition to start of path
        mlastPosition = mPoints[0];
    }

    mat4 getModelMatrix() override
    {
        float current_time = (float)glfwGetTime();

        // --- FIX 1: Initialize Timing on First Update ---
        // If this is the first frame for this specific object (original or copy),
        // snap last_time to current_time so we don't get a huge delta.
        if (mfirstUpdate)
        {
            last_time = current_time;
            // We assume position is at start, so delta is 0 this frame.
            // We treat this as "setup complete" and return identity 
            // so the object stays put for 1 frame while time aligns.
            mfirstUpdate = false;
            return mat4(1.0f);
        }

        // --- FIX 2: Calculate Delta Time ---
        float delta_time = current_time - last_time;
        last_time = current_time;

        // Prevent negative delta time (edge case protection)
        if (delta_time < 0.0f) delta_time = 0.0f;

        // --- Path Logic ---
        mCurrentT += mSpeedT * delta_time;

        // Ping-pong or Loop logic (Basic loop 0->1)
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

        // Current Absolute Position
        vec3 current_position = mix(P_i, P_i_plus_1, u);

        // --- Calculate Position Delta ---
        // Since TransformManager accumulates (multiplies), we return the *difference*
        // between where we were last frame and where we are now.
        vec3 delta_move = current_position - mlastPosition;

        // Update history for next frame
        mlastPosition = current_position;

        return translate(mat4(1.0f), delta_move);
    }

    TransformBase* getCopy() const override
    {
        // Standard copy
        auto* copy = new ParametricLineMovement(*this);

        // --- FIX 3: Force Reset on Copy ---
        // This ensures the new object treats its very first update frame
        // as a "reset time" frame, preventing the "huge jump" bug.
        copy->mfirstUpdate = true;

        return copy;
    }
};
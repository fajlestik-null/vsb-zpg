#pragma once
#include "Includes.h"

//NEED TO SEPARATE TO .h AND .cpp FILES!!! -> TODO (after I have all the transformations to my dispose)

// Base class for transformations - (use as instance DT)
class TransformBase {
public:
    virtual ~TransformBase() = default;
    virtual void apply(const vec3& delta) = 0;
    virtual mat4 getModelMatrix() const = 0;
};

//Child classes for specific transformations - (do not create instances directly!)

class Translation : public TransformBase {
private:
	vec3 mVector;
public:
    Translation(const vec3& vec = vec3(0.0f)) : mVector(vec) {}

    void apply(const vec3& delta) override {
        mVector += delta;
    }

    mat4 getModelMatrix() const override {
        return translate(mat4(1.0f), mVector);
    }
};

class Rotation : public TransformBase {
protected:
    vec3 mVector; 
public:
    Rotation(const vec3& angles = vec3(0.0f)) : mVector(angles) {}

    void apply(const vec3& delta) override {
        mVector += delta;
    }

    mat4 getModelMatrix() const override {
        mat4 rot = mat4(1.0f);
        rot = rotate(rot, radians(mVector.x), vec3(1, 0, 0));
        rot = rotate(rot, radians(mVector.y), vec3(0, 1, 0));
        rot = rotate(rot, radians(mVector.z), vec3(0, 0, 1));
        return rot;
    }
};

class Scaling : public TransformBase {
private:
    vec3 mVector;
public:
    Scaling(const vec3& vec = vec3(0.0f)) : mVector(vec) {}

    //x,z,y
    void apply(const vec3& delta) override {
        mVector += delta;
        
    }

    mat4 getModelMatrix() const override 
    { 
        return scale(mat4(1.0f), mVector);
    }
};

class TransformGroup : public TransformBase {
private:
    vector<TransformBase*> mTransforms;

public:
    void addTransform(TransformBase* transform)
    {
        mTransforms.push_back(transform);
    }
    void apply(const vec3& delta) override
    {
        for (auto transform : mTransforms) {
            transform->apply(delta);
        }
    }
    mat4 getModelMatrix() const override
    {
        mat4 result = mat4(1.0f);
        for (auto transform : mTransforms) {
            result = result * transform->getModelMatrix();
        }
        return result;
    }
};

//TODO
//Dynamic 
#pragma once
#include "Includes.h"

//NEED TO SEPARATE TO .h AND .cpp FILES!!! -> TODO (after I have all the transformations to my dispose)

// Base class for transformations - (use in vectors)
class Transformation {
protected:
    glm::vec3 mVector;
    glm::mat4 mTransMatrix;

public:
    Transformation() : mVector(0.0f), mTransMatrix(1.0f) {}

    Transformation(const glm::vec3& vec) : mVector(vec), mTransMatrix(1.0f) {}

    
    virtual ~Transformation() {}

    
    void setIdentity() {
        mVector = glm::vec3(0.0f);
        mTransMatrix = glm::mat4(1.0f);
    }


    virtual void apply(const glm::vec3& delta) = 0;


    virtual glm::mat4 getModelMatrix() const = 0;
};

//Child classes for specific transformations - (do not create instances directly!)

class Translation : public Transformation {
public:
    Translation() : Transformation() {}
    Translation(const glm::vec3& vec) : Transformation(vec) {
        mTransMatrix = glm::translate(glm::mat4(1.0f), mVector);
    }

    void apply(const glm::vec3& delta) override {
        mVector += delta;
        mTransMatrix = glm::translate(glm::mat4(1.0f), mVector);
    }

    glm::mat4 getModelMatrix() const override { return mTransMatrix; }
};

class Rotation : public Transformation {
public:
    Rotation() : Transformation() {}
    Rotation(const glm::vec3& angles) : Transformation(angles) {
        apply(glm::vec3(0.0f)); 
    }

    void apply(const glm::vec3& delta) override {
        mVector += delta;
        mTransMatrix = glm::mat4(1.0f);
        mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mVector.x), glm::vec3(1, 0, 0));
        mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mVector.y), glm::vec3(0, 1, 0));
        mTransMatrix = glm::rotate(mTransMatrix, glm::radians(mVector.z), glm::vec3(0, 0, 1));
    }

    glm::mat4 getModelMatrix() const override { return mTransMatrix; }
};

class Scaling : public Transformation {
public:
    Scaling() : Transformation(glm::vec3(1.0f)) {
        mTransMatrix = glm::scale(glm::mat4(1.0f), mVector);
    }

    Scaling(const glm::vec3& vec) : Transformation(vec) {
        mTransMatrix = glm::scale(glm::mat4(1.0f), mVector);
    }

    void apply(const glm::vec3& delta) override {
        mVector += delta;
        mTransMatrix = glm::scale(glm::mat4(1.0f), mVector);
    }

    glm::mat4 getModelMatrix() const override { return mTransMatrix; }
};

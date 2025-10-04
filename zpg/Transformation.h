#pragma once
#include "Includes.h"

//NEED TO SEPARATE TO .h AND .cpp FILES!!! -> TODO (after I have all the transformations to my dispose)

// Base class for transformations - (use in vectors)
class TransformBase {
protected:
    glm::vec3 mVector;
    glm::mat4 mTransMatrix;

public:
    TransformBase() : mVector(0.0f), mTransMatrix(1.0f) {}

    TransformBase(const glm::vec3& vec) : mVector(vec), mTransMatrix(1.0f) {}

    
    virtual ~TransformBase() {}

    
    void setIdentity() {
        mVector = glm::vec3(0.0f);
        mTransMatrix = glm::mat4(1.0f);
    }


    virtual void apply(const glm::vec3& delta) = 0;


    virtual glm::mat4 getModelMatrix() const = 0;
};

//Child classes for specific transformations - (do not create instances directly!)

class Translation : public TransformBase {
public:
    Translation() : TransformBase() {}
    Translation(const glm::vec3& vec) : TransformBase(vec) {
        mTransMatrix = glm::translate(glm::mat4(1.0f), mVector);
    }

    void apply(const glm::vec3& delta) override {
        mVector += delta;
        mTransMatrix = glm::translate(glm::mat4(1.0f), mVector);
    }

    glm::mat4 getModelMatrix() const override { return mTransMatrix; }
};

class Rotation : public TransformBase {
public:
    Rotation() : TransformBase() {}
    Rotation(const glm::vec3& angles) : TransformBase(angles) {
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

class Scaling : public TransformBase {
public:
    Scaling() : TransformBase(glm::vec3(1.0f)) {
        mTransMatrix = glm::scale(glm::mat4(1.0f), mVector);
    }

    Scaling(const glm::vec3& vec) : TransformBase(vec) {
        mTransMatrix = glm::scale(glm::mat4(1.0f), mVector);
    }

    void apply(const glm::vec3& delta) override {
        mVector += delta;
        mTransMatrix = glm::scale(glm::mat4(1.0f), mVector);
    }

    glm::mat4 getModelMatrix() const override { return mTransMatrix; }
};

//TODO
/*class Transform : public TransformBase {
    
private:
	std::vector<TransformBase*> mTransforms;
        
public:
        Transform() : TransformBase() {}
        void addTransform(TransformBase* transform) {
            mTransforms.push_back(transform);
        }
        void apply(const glm::vec3& delta) override {
            for (auto& transform : mTransforms) {
                transform->apply(delta);
            }
        }
        glm::mat4 getModelMatrix() const override {
            glm::mat4 result = glm::mat4(1.0f);
            for (const auto& transform : mTransforms) {
                result = result * transform->getModelMatrix();
            }
            return result;
		}
};*/

#pragma once

#include "Includes.h"
#include "IncludeTransformations.h"

class TransformManager : public enable_shared_from_this<TransformManager>
{
private:
    bool mCalculated;
    mat4 mFinalMatrix;

    mat4 mLocalDynamic;  // e.g. self-rotation
    mat4 mGlobalDynamic; // e.g. orbit motion

    vector<mat4> mStaticMatricies; // 3 matrixes: translation, rotation, scale

    vector<pair<TransformBase*, mat4>> mLocalTransforms;
    vector<pair<TransformBase*, mat4>> mGlobalTransforms;

    // Optional parent (for hierarchical transform)
    vector<weak_ptr<TransformManager>> mParents;


public:
	TransformManager() : mCalculated(false), mFinalMatrix(1.0f), mLocalDynamic(1.0f), mGlobalDynamic(1.0f), mStaticMatricies(4, mat4(1.0f))
    {
    
    }

    void addParent(shared_ptr<TransformManager> parentManager) {mParents.push_back(parentManager);}

    void calculateTransform()
    {
        // Accumulate dynamic transforms
        for (auto& t : mLocalTransforms)
            t.second *= t.first->getModelMatrix();

        for (auto& t : mGlobalTransforms)
            t.second *= t.first->getModelMatrix();

        mCalculated = false;
    }

    //bool hasParent() const { return !mParents.empty(); }

    mat4 getFinalMatrix()
    {
        if (!mCalculated)
        {
            mCalculated = true;

            mat4 mParentGlobal = mat4(1.0f);
            for (auto &parent : mParents)
            {
                if (auto p = parent.lock())
                    mParentGlobal *= p->getGlobalDynamic(); // inherit motion only
            }

            mFinalMatrix = mParentGlobal* getGlobalDynamic() * mStaticMatricies[0] * getLocalDynamic() * mStaticMatricies[1] * mStaticMatricies[2] * mStaticMatricies[3]; // PG * G * Ts * L * R * S
        }

        return mFinalMatrix ;
    }

    void addStaticTransform(TransformBase *t)
    {
        mCalculated = false;

        if (dynamic_cast<Translation *>(t))
        {
            mStaticMatricies[0] *= t->getModelMatrix();
        }
        else if (dynamic_cast<Rotation *>(t))
        {
            mStaticMatricies[1] *= t->getModelMatrix();
        }
        else if (dynamic_cast<Scaling *>(t))
        {
            mStaticMatricies[2] *= t->getModelMatrix();
        }
        else
        {
            mStaticMatricies[3] *= t->getModelMatrix();
        }

    }
    void addLocalTransform(TransformBase *t) { mLocalTransforms.push_back({t, mat4(1.0f)}); }
    void addGlobalTransform(TransformBase*t) { mGlobalTransforms.push_back({t, mat4(1.0f)}); }

    mat4 getLocalDynamic()
    {
        mLocalDynamic = mat4(1.0f);
        for (const auto &t : mLocalTransforms)
        {
            mLocalDynamic *= t.second;
        }
        return mLocalDynamic;
    }
    mat4 getGlobalDynamic()
    {
        mGlobalDynamic = mat4(1.0f);
        for (const auto &t : mGlobalTransforms)
        {
            mGlobalDynamic *= t.second;
        }
        return mGlobalDynamic;
    }
};

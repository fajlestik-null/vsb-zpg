#pragma once

#include "Includes.h"
#include "IncludeTransformations.h"

enum class InheritTransformation
{
    TRANSLATION,
	ROTATION,
	SCALE,
	ALL
};

class TransformManager : public enable_shared_from_this<TransformManager>
{
private:
    bool mCalculated;
    mat4 mFinalMatrix;

	InheritTransformation mInheritanceType = InheritTransformation::ALL;

    mat4 mLocalDynamic;  // e.g. self-rotation
    mat4 mGlobalDynamic; // e.g. orbit motion

	vector<mat4> mStaticMatricies; // 3 matrixes: translation, rotation, scale, custom, component

    vector<pair<TransformBase*, mat4>> mLocalTransforms;
    vector<pair<TransformBase*, mat4>> mGlobalTransforms;

    // Optional parent (for hierarchical transform)
    vector<weak_ptr<TransformManager>> mParents;


public:
	TransformManager() : mCalculated(false), mFinalMatrix(1.0f), mLocalDynamic(1.0f), mGlobalDynamic(1.0f), mStaticMatricies(5, mat4(1.0f))
    {
    
    }

	bool isCalculated() const {
        return mCalculated;
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
                {
                    if(mInheritanceType == InheritTransformation::TRANSLATION)
                    {
                        mat4 parentFinal = p->getFinalMatrix();
                        vec3 parentTranslation = vec3(parentFinal[3]);
                        mParentGlobal *= translate(mat4(1.0f), parentTranslation);
                    }
                    else if (mInheritanceType == InheritTransformation::ROTATION)
                    {
                        // Extract rotation matrix (upper-left 3x3)
                        mat4 parentFinal = p->getFinalMatrix();
                        mat3 rotationOnly = mat3(parentFinal);
                        rotationOnly[0] = normalize(rotationOnly[0]);
                        rotationOnly[1] = normalize(rotationOnly[1]);
                        rotationOnly[2] = normalize(rotationOnly[2]);
                        mParentGlobal *= mat4(rotationOnly);
                    }
                    else if (mInheritanceType == InheritTransformation::SCALE)
                    {
                        mat4 parentFinal = p->getFinalMatrix();
                        // Extract scale factors
                        vec3 scaleFactors = vec3(length(vec3(parentFinal[0])), length(vec3(parentFinal[1])), length(vec3(parentFinal[2])));
                        mParentGlobal *= glm::scale(mat4(1.0f), scaleFactors);
                    }
					else
                    {
                        mParentGlobal *= p->getFinalMatrix(); // inherit all of paret's transforms
                    }
                }
            }

            mFinalMatrix = mParentGlobal* getGlobalDynamic() * mStaticMatricies[0] * getLocalDynamic() * mStaticMatricies[1] * mStaticMatricies[2] * mStaticMatricies[4] * mStaticMatricies[3]; // PG * G * Ts * L * R * S
        }

        return mFinalMatrix ;
    }

    void setFinalMatrix(const mat4& matrix)
    {
        mFinalMatrix = matrix;
        mCalculated = true;
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
        else if (dynamic_cast<Custom *>(t))
        {
            mStaticMatricies[3] *= t->getModelMatrix();
        }
        else
        {
            mStaticMatricies[4] *= t->getModelMatrix();
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

	void setInheritOnlyTransform(const InheritTransformation VALUE) { mInheritanceType = VALUE; }

    shared_ptr<TransformManager> getCopy() const
    {
        shared_ptr<TransformManager> duplicate = make_shared<TransformManager>();

        for (const auto& local : mLocalTransforms)
        {
            duplicate->addLocalTransform(local.first->getCopy());
        }

        for (const auto& global : mGlobalTransforms)
        {
            duplicate->addGlobalTransform(global.first->getCopy());
        }

        duplicate->mStaticMatricies = this->mStaticMatricies;

		return duplicate;
    }
};

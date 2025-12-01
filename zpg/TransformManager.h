#pragma once

#include "Includes.h"
#include "IncludeTransformations.h"
#include "InheritTransformationType.h"



class TransformManager : public enable_shared_from_this<TransformManager>
{
private:
    bool mCalculated;
    mat4 mFinalMatrix;

    InheritTransformation mInheritanceType = InheritTransformation::ALL;

    mat4 mLocalDynamic;
    mat4 mGlobalDynamic;

    vector<mat4> mStaticMatricies;

    vector<pair<TransformBase*, mat4>> mLocalTransforms;
    vector<pair<TransformBase*, mat4>> mGlobalTransforms;

    vector<weak_ptr<TransformManager>> mParents;

public:
    TransformManager();

    bool isCalculated() const;

    void addParent(shared_ptr<TransformManager> parentManager);

    void calculateTransform();

    mat4 getFinalMatrix();

    void setFinalMatrix(const mat4& matrix);

    void addStaticTransform(TransformBase* t);

    void addLocalTransform(TransformBase* t);
    void addGlobalTransform(TransformBase* t);

    mat4 getLocalDynamic();
    mat4 getGlobalDynamic();

    void setInheritOnlyTransform(const InheritTransformation VALUE);

    shared_ptr<TransformManager> getCopy() const;

    void clearDynamicTransforms();

    void clearStaticTransforms();

    void clearStaticTranslations();
};
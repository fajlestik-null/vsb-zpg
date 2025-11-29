#include "TransformManager.h"

TransformManager::TransformManager() : mCalculated(false), mFinalMatrix(1.0f), mLocalDynamic(1.0f), mGlobalDynamic(1.0f), mStaticMatricies(5, mat4(1.0f))
{
}

bool TransformManager::isCalculated() const
{
    return mCalculated;
}

void TransformManager::addParent(shared_ptr<TransformManager> parentManager)
{
    mParents.push_back(parentManager);
}

void TransformManager::calculateTransform()
{
    for (auto& t : mLocalTransforms)
        t.second *= t.first->getModelMatrix();

    for (auto& t : mGlobalTransforms)
        t.second *= t.first->getModelMatrix();

    mCalculated = false;
}

mat4 TransformManager::getFinalMatrix()
{
    if (!mCalculated)
    {
        mCalculated = true;

        mat4 mParentGlobal = mat4(1.0f);
        for (auto& parent : mParents)
        {
            if (auto p = parent.lock())
            {
                if (mInheritanceType == InheritTransformation::TRANSLATION)
                {
                    mat4 parentFinal = p->getFinalMatrix();
                    vec3 parentTranslation = vec3(parentFinal[3]);
                    mParentGlobal *= translate(mat4(1.0f), parentTranslation);
                }
                else if (mInheritanceType == InheritTransformation::ROTATION)
                {
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
                    vec3 scaleFactors = vec3(length(vec3(parentFinal[0])), length(vec3(parentFinal[1])), length(vec3(parentFinal[2])));
                    mParentGlobal *= glm::scale(mat4(1.0f), scaleFactors);
                }
                else
                {
                    mParentGlobal *= p->getFinalMatrix();
                }
            }
        }

        mFinalMatrix = mParentGlobal * getGlobalDynamic() * mStaticMatricies[0] * getLocalDynamic() * mStaticMatricies[1] * mStaticMatricies[2] * mStaticMatricies[4] * mStaticMatricies[3];
    }

    return mFinalMatrix;
}

void TransformManager::setFinalMatrix(const mat4& matrix)
{
    mFinalMatrix = matrix;
    mCalculated = true;
}

void TransformManager::addStaticTransform(TransformBase* t)
{
    mCalculated = false;

    if (dynamic_cast<Translation*>(t))
    {
        mStaticMatricies[0] *= t->getModelMatrix();
    }
    else if (dynamic_cast<Rotation*>(t))
    {
        mStaticMatricies[1] *= t->getModelMatrix();
    }
    else if (dynamic_cast<Scaling*>(t))
    {
        mStaticMatricies[2] *= t->getModelMatrix();
    }
    else if (dynamic_cast<Custom*>(t))
    {
        mStaticMatricies[3] *= t->getModelMatrix();
    }
    else
    {
        mStaticMatricies[4] *= t->getModelMatrix();
    }
}

void TransformManager::addLocalTransform(TransformBase* t)
{
    mLocalTransforms.push_back({ t, mat4(1.0f) });
}

void TransformManager::addGlobalTransform(TransformBase* t)
{
    mGlobalTransforms.push_back({ t, mat4(1.0f) });
}

mat4 TransformManager::getLocalDynamic()
{
    mLocalDynamic = mat4(1.0f);
    for (const auto& t : mLocalTransforms)
    {
        mLocalDynamic *= t.second;
    }
    return mLocalDynamic;
}

mat4 TransformManager::getGlobalDynamic()
{
    mGlobalDynamic = mat4(1.0f);
    for (const auto& t : mGlobalTransforms)
    {
        mGlobalDynamic *= t.second;
    }
    return mGlobalDynamic;
}

void TransformManager::setInheritOnlyTransform(const InheritTransformation VALUE)
{
    mInheritanceType = VALUE;
}

shared_ptr<TransformManager> TransformManager::getCopy() const
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

void TransformManager::clearDynamicTransforms()
{
    for (auto& t : mLocalTransforms)
        delete t.first;
    for (auto& t : mGlobalTransforms)
        delete t.first;

    mLocalTransforms.clear();
    mGlobalTransforms.clear();
    mCalculated = false;
}

void TransformManager::clearStaticTransforms()
{
    mStaticMatricies = vector<mat4>(5, mat4(1.0f));
    mCalculated = false;
}

void TransformManager::clearStaticTranslations()
{
    mStaticMatricies[0] = mat4(1.0f);
    mCalculated = false;
}
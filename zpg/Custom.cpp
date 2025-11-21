#include "Custom.h"

Custom::Custom(const mat4& mat) : mMatrix(mat)
{
}

mat4 Custom::getModelMatrix()
{
	return mMatrix;
}

TransformBase* Custom::getCopy() const
{
	return new Custom(*this);
}

#include "WorldEntityGenerator.h"

void WorldEntityGenerator::setSampleEntity(WorldEntity* sampleEntity)
{
	mSampleEntity = sampleEntity;
}

void WorldEntityGenerator::setMinRange(const vec3& MINRANGE)
{
	mMinRange = MINRANGE;
}

void WorldEntityGenerator::setMaxRange(const vec3& MAXRANGE)
{
	mMaxRange = MAXRANGE;
}

WorldEntity* WorldEntityGenerator::generateEntity(const vec3 POSITION)
{
	auto* newEntity = mSampleEntity->getCopy();
	newEntity->addStaticTransform(new Translation(POSITION));
	return newEntity;
}

WorldEntity* WorldEntityGenerator::generateEntityRandomly()
{
	auto* newEntity = mSampleEntity->getCopy();
	newEntity->addStaticTransform(new Translation(
		vec3(
			mMinRange.x + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mMaxRange.x - mMinRange.x))),
			mMinRange.y + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mMaxRange.y - mMinRange.y))),
			mMinRange.z + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (mMaxRange.z - mMinRange.z)))
		)
	));
	return newEntity;
}

#pragma once
#include "Includes.h"
#include "WorldEntity.h"

class WorldEntityGenerator
{
private:
	WorldEntity* mSampleEntity;
	vec3 mMinRange;
	vec3 mMaxRange;
	vector<vec3> mPointsForBezier;
public:
	WorldEntityGenerator();
	~WorldEntityGenerator() = default;

	void setSampleEntity(WorldEntity* sampleEntity);
	void setMinRange(const vec3& MINRANGE);
	void setMaxRange(const vec3& MAXRANGE);

	void addPointForBezier(const vec3 POINT);
	void clearPointsForBezier();

	WorldEntity* generateBezierEntity();

	WorldEntity* generateEntity(const vec3 POSITION);

	WorldEntity* generateEntityRandomly();

};
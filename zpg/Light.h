#pragma once
#include "WorldEntity.h"


class Light : public WorldEntity
{
private:
	int mID;
	float mIntensity;
	vec3 mLightColor;
public:
	static int LIGHT_COUNT;
	SubjectType getType() const override;

	~Light();
	Light(const vec3 LIGHT_COLOR, const float INTENSITY);
	Light(Model* model,ShaderProgram* shader,vec3 color);

	void update(GLFWwindow* window, float delta_time, Controls* controls);

	vec3 getLightColor() const;
	float getIntensity() const;
	int getID() const;
};
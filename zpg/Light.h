#pragma once
#include "WorldEntity.h"
#include "LightType.h"


class Light : public WorldEntity
{
private:
	int mID;
	float mIntensity;
	float mDistance;
	vec3 mLightColor;
	bool mActive;
	LightType mLightType;
public:
	static int LIGHT_COUNT;
	SubjectType getType() const override;

	~Light();
	Light(Model* model,ShaderProgram* shader,vec3 color);
	Light(const LightType LIGHT_TYPE, const vec3 LIGHT_COLOR, const float INTENSITY, const float DISTANCE);

	void processKeyboard(GLFWwindow* window, float deltaTime, Controls* controls);
	void update(GLFWwindow* window, float delta_time, Controls* controls);

	vec3 getLightColor() const;
	float getIntensity() const;
	int getLightType() const;
	float getDistance() const;
	bool isActive() const;
	void setActive(const bool ACTIVE);
	int getID() const;
};
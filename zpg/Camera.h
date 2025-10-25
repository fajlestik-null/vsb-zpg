#pragma once
#include "WorldEntity.h"


class Camera: public WorldEntity
{
private:
	mat4 mProjectionMatrix;
	mat4 mViewMatrix;

	float mAlpha;
	float mPhi;

	vec3 mEye;
	vec3 mTarget;
	vec3 mUp;

	vec3 mWorldUp;
	vec3 mRight;

	float mWindowWidth;
	float mWindowHeight;

public:
	Camera();
	~Camera();

	void processKeyboard(GLFWwindow* window, float deltaTime, Controls* controls);
	void processMouse(double xOffset, double yOffset);
	void updateWindowSize(const float WINDOW_WIDTH, const float WINDOW_HEIGHT);
	void recalculateCameraVectors();
	void update(GLFWwindow* window, float deltaTime, Controls* controls) override;
	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
	vec3 getPosition();

	SubjectType getType() const override;
};

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

	int mWindowWidth;
	int mWindowHeight;

public:
	Camera();
	~Camera();

	void processKeyboard(GLFWwindow* window, float deltaTime, Controls* controls);
	void getStencilPosition(GLFWwindow* window, Controls* constrols);
	void processMouse(double xOffset, double yOffset);
	void recalculateCameraVectors();
	void update(GLFWwindow* window, float deltaTime, Controls* controls) override;
	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
	vec3 getPosition();

	SubjectType getType() const override;
};

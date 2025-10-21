#pragma once
#include "Includes.h"
#include "Subject.h"
#include "Controls.h"


class Camera:public Subject
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
	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
	vec3 getPosition();
};

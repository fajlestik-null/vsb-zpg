#pragma once
#include "Includes.h"
#include "ICameraObserver.h"
#include "Controls.h"
using namespace std;
using namespace glm;

class Camera
{
private:
	mat4 mProjectionMatrix = mat4(1.0f);
	mat4 mViewMatrix = mat4(1.0f);

	float mAlpha = 0.0f;
	float mPhi = 0.0f; 

	vec3 mEye = vec3(0.0f, 0.0f, 5.0f);
	vec3 mTarget = vec3(0.0f, 0.0f, -1.0f);
	vec3 mUp = vec3(0.0f, 1.0f, 0.0f);

	vec3 mWorldUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 mRight = vec3(0.0f);

	float mWindowWidth = 800.0f;
	float mWindowHeight = 600.0f;

	vector<ICameraObserver*> mObservers;

public:
	Camera();
	~Camera() {};
	void attach(ICameraObserver* observer);
	void detach(ICameraObserver* observer);
	void notifyObservers();
	void processKeyboard(GLFWwindow* window, float deltaTime, Controls* controls);
	void processMouse(double xOffset, double yOffset);
	void updateWindowSize(const float WINDOW_WIDTH, const float WINDOW_HEIGHT);
	void recalculateCameraVectors();
	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
};

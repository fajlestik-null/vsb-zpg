#include "Camera.h"

void Camera::attach(ICameraObserver* observer)
{
	mObservers.push_back(observer);
}
void Camera::detach(ICameraObserver* observer)
{
	mObservers.erase(remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}

void Camera::notifyObservers(){
	for (ICameraObserver* observer : mObservers) {
		observer->onCameraChanged(this);
	}
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime, Controls* controls)
{
    float movementSpeed = 2.5f;
    float velocity = movementSpeed * deltaTime;

    if (controls->isKeyPressed(GLFW_KEY_W))
        mEye += mTarget * velocity;
    if (controls->isKeyPressed(GLFW_KEY_S))
        mEye -= mTarget * velocity;
    if (controls->isKeyPressed(GLFW_KEY_A))
        mEye -= mRight * velocity;
    if (controls->isKeyPressed(GLFW_KEY_D))
        mEye += mRight * velocity;
    if (controls->isKeyPressed(GLFW_KEY_SPACE))
        mEye += mWorldUp * velocity;
    if (controls->isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        mEye -= mWorldUp * velocity;
}

void Camera::processMouse(double xOffset, double yOffset)
{
	float mouse_sensitivity = 0.1f;
    xOffset *= mouse_sensitivity;
    yOffset *= mouse_sensitivity;

    fi += (float)xOffset;
    alfa += (float)yOffset;

	if (fi > radians(89.0f))
		fi = radians(89.0f);
	if (fi < radians(-89.0f))
		fi = radians(-89.0f);
	mTarget.x = cos(fi) * sin(alfa);
	mTarget.y = sin(alfa);
	mTarget.z = sin(fi) * cos(alfa);
}

mat4 Camera::getViewMatrix(void)
{
	mViewMatrix = lookAt(mEye, mEye + mTarget, mUp);
		return mViewMatrix;
}

mat4 Camera::getProjectionMatrix()
{
	//60° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mProjectionMatrix = perspective(radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	return mProjectionMatrix;
}
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

	//Maybe will need to move
	mRight = normalize(cross(mTarget, mWorldUp));  //problem -> walking diagonally
	mUp = normalize(cross(mRight, mTarget));

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
	float mouse_sensitivity = 0.05f;
	xOffset *= mouse_sensitivity;
	yOffset *= mouse_sensitivity;

	mPhi += (float)xOffset;
	mAlpha += (float)yOffset;

	if (mAlpha > radians(89.0f))
		mAlpha = radians(89.0f);
	if (mAlpha < radians(-89.0f))
		mAlpha = radians(-89.0f);

	//Maybe will need to move
	float radius = 1.0f; // Distance from the origin -> normalized to unit sphere

	mTarget.x = radius * cos(mPhi) * sin(mAlpha);
	mTarget.y = radius * cos(mAlpha);
	mTarget.z = radius * sin(mPhi) * sin(mAlpha);

}

void Camera::updateWindowSize(const float WINDOW_WIDTH, const float WINDOW_HEIGHT)
{
	mWindowWidth = WINDOW_HEIGHT;
	mWindowHeight = WINDOW_HEIGHT;
}

mat4 Camera::getViewMatrix(void)
{
	mViewMatrix = lookAt(mEye, mEye + mTarget, mUp);
		return mViewMatrix;
}

mat4 Camera::getProjectionMatrix(void)
{
	//60° Field of View, window ratio (default -> 4:3 ratio), display range : 0.1 unit <-> 100 units
	mProjectionMatrix = perspective(radians(60.0f), mWindowWidth / mWindowHeight, 0.1f, 100.0f);
	return mProjectionMatrix;
}

void Camera::recalculateCameraVectors()
{
	// Also re-calculate the Right and Up vector

}
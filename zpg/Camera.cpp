#include "Camera.h"

Camera::Camera() : mProjectionMatrix(mat4(1.0f)), mViewMatrix(mat4(1.0f)),
mPhi(radians(-90.0f)), mAlpha(radians(90.0f)),
mEye(vec3(0.0f, 0.0f, 5.0f)), mTarget(vec3(0.0f, 0.0f, 0.0f)), mUp(vec3(0.0f, 1.0f, 0.0f)),
mWorldUp(vec3(0.0f, 1.0f, 0.0f)), mRight(vec3(0.0f)),
mWindowWidth(800.0f), mWindowHeight(600.0f)
{
	this->setTexture(nullptr);
}

Camera::~Camera()
{
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime, Controls* controls)
{
    float movementSpeed = 2.5f;
    float velocity = movementSpeed * deltaTime;

	mRight = normalize(cross(mTarget, mWorldUp));  
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

void Camera::getStencilPosition(GLFWwindow* window, Controls * constrols)
{
	GLbyte color[4];
	GLfloat depth;
	GLuint index;

	GLint x = (GLint)constrols->getMouseX();
	GLint y = (GLint)constrols->getMouseY();

	int newy = mWindowHeight - y;

	glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	if (mDebugFlag)
	{
		cout << "Clicked on pixel " << x << ", " << y << ", color " << hex << (int)color[0] << (int)color[1] << (int)color[2] << (int)color[3] << dec << ", depth " << depth << ", stencil index " << index << endl;
	}

	vec3 pos = vec3(0.0f);

	if (depth != 1)
	{
		vec3 screenX = vec3(x, newy, depth);

		vec4 viewPort = vec4(0, 0, mWindowWidth, mWindowHeight);
		pos = unProject(screenX, mViewMatrix, mProjectionMatrix, viewPort);
	}

	if (mDebugFlag)
	{
		cout << "unProject " << pos.x << " " << pos.y << " " << pos.z;
	}

	constrols->setPosition(pos);
	constrols->setStencilIndex(index);
}

void Camera::processMouse(double xOffset, double yOffset)
{
	float mouse_sensitivity = 0.05f;
	xOffset *= mouse_sensitivity;
	yOffset *= mouse_sensitivity;

	mPhi += (float)xOffset;
	mAlpha -= (float)yOffset;

	float alphaMin = radians(1.0f);
	float alphaMax = radians(179.0f);

	if (mAlpha < alphaMin) mAlpha = alphaMin;
	if (mAlpha > alphaMax) mAlpha = alphaMax;
}

mat4 Camera::getViewMatrix(void)
{
		return mViewMatrix;
}

vec3 Camera::getPosition()
{
	return mEye;
}

void Camera::setPosition(const vec3 POSITION)
{
	mEye = POSITION;
}

SubjectType Camera::getType() const
{ 
	return SubjectType::CAMERA;
};

mat4 Camera::getProjectionMatrix(void)
{
	return mProjectionMatrix;
}



void Camera::recalculateCameraVectors()
{
	float radius = 1.0f; // Distance from the origin -> normalized to unit sphere

	vec3 tmpTarget;
	tmpTarget.x = radius * sin(mAlpha) * cos(mPhi);
	tmpTarget.y = radius * cos(mAlpha);
	tmpTarget.z = radius * sin(mAlpha) * sin(mPhi);
	mTarget = normalize(tmpTarget);


	mRight = normalize(cross(mTarget, mWorldUp));
	mUp = normalize(cross(mRight, mTarget));

	mViewMatrix = lookAt(mEye, mEye + mTarget, mUp);
	//Zero division protection
	if (mWindowWidth == 0 || mWindowHeight == 0)
		return;

	float ratio = (float)mWindowWidth / (float)mWindowHeight;

	//60° Field of View, window ratio (default -> 4:3 ratio), display range : 0.1 unit <-> 100 units
	mProjectionMatrix = perspective(radians(60.0f), ratio, 0.1f, 100.0f);
	}

void Camera::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
	this->processKeyboard(window, deltaTime, controls);
	if (controls->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		this->processMouse(controls->getMouseDeltaX(), controls->getMouseDeltaY());
	}

	glfwGetWindowSize(window, &mWindowWidth, &mWindowHeight);

	this->recalculateCameraVectors();

	mat4 cameraFollowMatrix = inverse(mViewMatrix);

	this->getTransformManager()->setFinalMatrix(cameraFollowMatrix);

	this->getStencilPosition(window, controls);
}

void Camera::turnDebugOn()
{
	mDebugFlag = true;
}

void Camera::turnDebugOff()
{
	mDebugFlag = false;
}

#include "Controls.h"

std::unordered_map<int, bool> Controls::keys;
std::unordered_map<int, bool> Controls::mouseButtons;

double Controls::mouseX = 0.0;
double Controls::mouseY = 0.0;
double Controls::lastMouseX = 0.0;
double Controls::lastMouseY = 0.0;
double Controls::mouseDeltaX = 0.0;
double Controls::mouseDeltaY = 0.0;
bool Controls::firstMouse = true;

Controls::Controls() {}

void Controls::attachToWindow(GLFWwindow* window)
{
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowUserPointer(window, this);
}

bool Controls::isKeyPressed(int key) const
{
    auto it = keys.find(key);
    return it != keys.end() && it->second;
}

bool Controls::isKeyReleased(int key) const
{
    auto it = keys.find(key);
    return it == keys.end() || !it->second;
}

bool Controls::isMouseButtonPressed(int button) const
{
    auto it = mouseButtons.find(button);
    return it != mouseButtons.end() && it->second;
}

void Controls::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void Controls::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        mouseButtons[button] = true;
    else if (action == GLFW_RELEASE)
        mouseButtons[button] = false;
}

void Controls::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    mouseX = xPos;
    mouseY = yPos;

    if (firstMouse)
    {
        lastMouseX = xPos;
        lastMouseY = yPos;
        firstMouse = false;
    }

    mouseDeltaX = xPos - lastMouseX;
    mouseDeltaY = lastMouseY - yPos;

    lastMouseX = xPos;
    lastMouseY = yPos;
}

// Ratio keeper
void Controls::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    float ratio = 4.0f / 3.0f;

	float fWidth = (float)width;
	float fHeight = (float)height;

    int viewportWidth = width;
    int viewportHeight = height;

    float heightOfViewport = fWidth / ratio;

    if (heightOfViewport > fHeight)
    {
        float widthOfViewport = fHeight * ratio;

        viewportWidth = static_cast<int>(widthOfViewport);
        viewportHeight = height;

    }
    else
    {
        // If the required height is smaller or equal, the limiting factor is the width.
        viewportWidth = width;
        viewportHeight = static_cast<int>(heightOfViewport);
    }

    int xOffset = (width - viewportWidth) / 2;
    int yOffset = (height - viewportHeight) / 2;

    glViewport(xOffset, yOffset, viewportWidth, viewportHeight);
}

void Controls::resetMouseDelta()
{
    mouseDeltaX = 0.0;
    mouseDeltaY = 0.0;
}

void Controls::errorCallback(int error, const char* description) { std::cout << "Error: " << description << std::endl; }
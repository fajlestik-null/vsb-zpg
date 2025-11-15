#include "Controls.h"

unordered_map<int, bool> Controls::keys;
unordered_map<int, bool> Controls::mouseButtons;
unordered_map<int, bool> Controls::notedKeys;
unordered_map<int, bool> Controls::notedButtons;


double Controls::mouseX = 0.0;
double Controls::mouseY = 0.0;
double Controls::lastMouseX = 0.0;
double Controls::lastMouseY = 0.0;
double Controls::mouseDeltaX = 0.0;
double Controls::mouseDeltaY = 0.0;
bool Controls::firstMouse = true;
vec3 Controls::sPosition = vec3(0.0f, 0.0f, 0.0f);
GLuint Controls::sStencilIndex = 0;

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

bool Controls::isKeyTriggered(int key) const
{
    return isKeyPressed(key) && !(notedKeys.find(key) != notedKeys.end() && notedKeys.at(key));
}

bool Controls::isMouseButtonPressed(int button) const
{
    auto it = mouseButtons.find(button);
    return it != mouseButtons.end() && it->second;
}

bool Controls::isMouseButtonTriggered(int button) const
{
    return isMouseButtonPressed(button) && !(notedButtons.find(button) != notedButtons.end() && notedButtons.at(button));
}

void Controls::nextFrame()
{
	notedKeys = keys;
	notedButtons = mouseButtons;
	resetMouseDelta();
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
    glViewport(0, 0, width, height);
}

void Controls::resetMouseDelta()
{
    mouseDeltaX = 0.0;
    mouseDeltaY = 0.0;
}

void Controls::errorCallback(int error, const char* description) { std::cout << "Error: " << description << std::endl; }
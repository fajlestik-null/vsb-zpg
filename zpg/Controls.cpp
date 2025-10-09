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
    mouseDeltaY = lastMouseY - yPos; // Invert Y so moving up looks up

    lastMouseX = xPos;
    lastMouseY = yPos;
}

void Controls::resetMouseDelta()
{
    mouseDeltaX = 0.0;
    mouseDeltaY = 0.0;
}

void Controls::errorCallback(int error, const char* description) { std::cout << "Error: " << description << std::endl; }
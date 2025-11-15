#pragma once

#include "Includes.h"

class Controls
{
private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    static unordered_map<int, bool> keys;
    static unordered_map<int, bool> mouseButtons;
    static unordered_map<int, bool> notedKeys;
    static unordered_map<int, bool> notedButtons;


    static double mouseX;
    static double mouseY;
    static double lastMouseX;
    static double lastMouseY;
    static double mouseDeltaX;
    static double mouseDeltaY;
    static bool firstMouse;
    static vec3 sPosition;
	static GLuint sStencilIndex;


public:
    Controls();

    void attachToWindow(GLFWwindow* window);

    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;
	bool isKeyTriggered(int key) const;
    bool isMouseButtonPressed(int button) const;
	bool isMouseButtonTriggered(int button) const;
	void nextFrame();

    double getMouseX() const {
        return mouseX;
    }
    double getMouseY() const {
            return mouseY;
    }
    double getMouseDeltaX() const { return mouseDeltaX; }
    double getMouseDeltaY() const { return mouseDeltaY; }
	vec3 getPosition() const { return sPosition; }
	void setPosition(const vec3& position) { sPosition = position; }
	GLuint getStencilIndex() const { return sStencilIndex; }
	void setStencilIndex(const GLuint index) { sStencilIndex = index; }

    static void errorCallback(int error, const char* description);

    // Reset mouse delta after using it
    void resetMouseDelta();
};
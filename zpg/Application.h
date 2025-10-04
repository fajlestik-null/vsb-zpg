#pragma once
#include "Includes.h"
#include "Scene.h"

class Application
{

private:
    GLFWwindow* mWindow;
    int mWindowWidth;
    int mWindowHeight;
    std::vector<Scene> mScenes;
	Scene *mGeneralScene;


public:
    Application(const int windoWidth, const int windowHeight);
    ~Application();
    bool init();
    void run();
    void switchScene(std::vector<Scene* (*)()> scenes);
    GLFWwindow* GetWindow() const { return mWindow; }

private:
    // Callbacks
    static void errorCallback(int error, const char* description);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void windowFocusCallback(GLFWwindow* window, int focused);

    static void windowIconifyCallback(GLFWwindow* window, int iconified);

    static void windowSizeCallback(GLFWwindow* window, int width, int height);

    static void cursorCallback(GLFWwindow* window, double x, double y);

    static void buttonCallback(GLFWwindow* window, int button, int action, int mode);
};
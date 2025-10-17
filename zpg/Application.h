#pragma once
#include "Includes.h"
#include "Scene.h"

class Application
{

private:
    GLFWwindow* mWindow;
	Controls* mControls = new Controls();
    int mWindowWidth;
    int mWindowHeight;
    vector<Scene> mScenes;
	Scene *mGeneralScene;


public:
    Application(const int windoWidth, const int windowHeight);
    ~Application();
    bool init();
    void run();
    void switchScene(vector<Scene* (*)()> scenes);
    GLFWwindow* GetWindow() const { return mWindow; }
};
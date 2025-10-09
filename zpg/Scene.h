#pragma once
#include "Includes.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "Camera.h"
#include "./Res/sphere.h"
#include "./Res/suzi_smooth.h"
#include "./Res/bushes.h"
#include "./Res/tree.h"


class Scene
{
private:
	std::vector<DrawableObject*> mDrawableObjects;
	Camera* mCamera = new Camera();
public:
	Scene(){};
    void addObject(DrawableObject* drawableObject) { mDrawableObjects.push_back(drawableObject); }
	void addCameraObserver(ICameraObserver* observer) { mCamera->attach(observer); }
	void processCamera(GLFWwindow * window, Controls * controls);
	void render();

};

Scene* sceneDefault();

Scene* sceneTriangle();

Scene* sceneSpheres();

Scene* sceneMess();

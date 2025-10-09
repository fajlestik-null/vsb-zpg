#pragma once
#include "Includes.h"

#include "ArraysOfVertices.h"

#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "Camera.h"





class Scene
{
private:
	std::vector<DrawableObject*> mDrawableObjects;
	Camera* mCamera = new Camera();
public:
	Scene(){};
    void addObject(DrawableObject* drawableObject) { mDrawableObjects.push_back(drawableObject); }
	void addCameraObserver(ICameraObserver* observer) { mCamera->attach(observer); }
	void processCamera(GLFWwindow* window, const float WINDOW_WIDTH, const float WINDOW_HEIGHT, Controls* controls);
	void render();

};

Scene* sceneDefault();

Scene* sceneTriangle();

Scene* sceneSpheres();

Scene* sceneMess();

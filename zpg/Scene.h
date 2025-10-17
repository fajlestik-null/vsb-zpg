#pragma once
#include "Includes.h"

#include "ArraysOfVertices.h"

#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
private:
	vector<DrawableObject*> mDrawableObjects;
	Camera* mCamera = new Camera();
	Light* mLight = new Light();
public:
	Scene(){};
    void addObject(DrawableObject* drawableObject) { mDrawableObjects.push_back(drawableObject); }
	void addCameraObserver(IObserver* observer) { mCamera->attach(observer); }
	void addLightObserver(IObserver* observer) { mLight->attach(observer); mLight->notifyObservers();}
	void setLight(Light* light, IObserver* observer) 
	{ 
	mLight = light;
	addLightObserver(observer);
	}
	void processCamera(GLFWwindow* window, const float WINDOW_WIDTH, const float WINDOW_HEIGHT, Controls* controls);
	void render();

};

Scene* sceneDefault();

Scene* sceneSpheres();

Scene* sceneTreesAndBushes();

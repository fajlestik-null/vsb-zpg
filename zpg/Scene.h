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
	vector<Light*> mLights = {};
public:
	Scene(){};
    void addObject(DrawableObject* drawableObject) { mDrawableObjects.push_back(drawableObject); }
	void addCameraObserver(IObserver* observer) { mCamera->attach(observer); }
	void addLightObserver(IObserver* observer) 
	{ 
		for (auto& light : mLights) 
		{ 
			light->attach(observer);
			light->notifyObservers();
		} 
	}
	void addLight(Light* light, IObserver* observer) 
	{ 
		mLights.push_back(light);
		addLightObserver(observer);
	}
	void processCamera(GLFWwindow* window, const float WINDOW_WIDTH, const float WINDOW_HEIGHT, Controls* controls);
	void render();

};

Scene* sceneDefault();

Scene* sceneSpheres();

Scene* sceneTreesAndBushes();

Scene* sceneSolarSystem();
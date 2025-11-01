#pragma once

#include "ArraysOfVertices.h"


#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"
#include "ResourceManager.h"

class Scene
{
private:
	vector<WorldEntity*> mWorldEntities;
public:
	Scene(){};
	~Scene();
	void addEntity(WorldEntity* entity) { mWorldEntities.push_back(entity); }

	void update(GLFWwindow* window, float deltaTime, Controls* controls);
	void render();

};

Scene* sceneDefault();

Scene* sceneSpheres();

Scene* sceneTreesAndBushes();

Scene* sceneSolarSystem();

Scene* sceneResourceLoadingTest();
#pragma once

#include "ArraysOfVertices.h"
#include "DrawableObject.h"
#include "Camera.h"
#include "Light.h"
#include "ResourceManager.h"

class Scene
{
private:
	WorldEntity* mSkyBox;
	WorldEntity* mActiveCamera;
	WorldEntity* mEntitityToHandle;
	vector<WorldEntity*> mWorldEntities;
public:
	Scene():mSkyBox(nullptr){};
	~Scene();
	void addEntity(WorldEntity* entity) { 
		mWorldEntities.push_back(entity);

		if (Camera* camera = dynamic_cast<Camera*>(entity)) {
			mActiveCamera = camera;
		}
	}

	void render(GLFWwindow* window, float deltaTime, Controls* controls);
	WorldEntity* getSkyBox() const { return mSkyBox; } // useless
	WorldEntity* getActiveCamera() const { return mActiveCamera; } // useless
	
	void setSkyBox(WorldEntity* skyBox) { mSkyBox = skyBox; }
	void setActiveCamera(WorldEntity* camera) { mActiveCamera = camera; }
	void setEntityToHandle(WorldEntity* entity) { mEntitityToHandle = entity; }
	void setSelectedEntity(Controls* controls);
	void removeEntity(Controls* controls);
	void insertEntity(Controls* controls);
	void transformEntity(Controls* controls);
};

Scene* sceneDefault();

Scene* sceneSpheres();

Scene* sceneTreesAndBushes();

Scene* sceneSolarSystem();

Scene* sceneTesting();
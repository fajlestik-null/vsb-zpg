#pragma once
#include "Includes.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"
#include "./Res/sphere.h"
#include "./Res/suzi_smooth.h"
#include "./Res/bushes.h"


class Scene
{
private:
	std::vector<DrawableObject*> mDrawableObjects;
public:
	Scene(){};
    void addObject(DrawableObject* drwobj) { mDrawableObjects.push_back(drwobj); }
	void render();

};

Scene* sceneDefault();

Scene* sceneTriangle();

Scene* sceneSpheres();

Scene* sceneMess();

#include "Scene.h"

void Scene::processCamera(GLFWwindow* window ,const float WINDOW_WIDTH, const float WINDOW_HEIGHT, Controls* controls)
{
	mCamera->updateWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	mCamera->processKeyboard(window, 0.016f, controls);
    if (controls->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        mCamera->processMouse(controls->getMouseDeltaX(), controls->getMouseDeltaY());
    }
    controls->resetMouseDelta();

   mCamera->recalculateCameraVectors();

	mCamera->notifyObservers();
}

void Scene::render()
{
    for (auto& obj : mDrawableObjects)
    {
        obj->draw();
    }
}

Scene* sceneDefault()
{
    vector<float>triangle = {
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    Scene* s = new Scene();
    IObserver* observer = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
    s->addCameraObserver(observer);
    DrawableObject* object = new DrawableObject(new Model(triangle), (ShaderProgram*)observer);
    s->addObject(object);
    return s;
}

Scene* sceneSpheres()
{
    Scene* scene = new Scene();

    Model* modelSphere;
    IObserver* observerConstant = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
    IObserver* observerLambert = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "lambert.frag");
    IObserver* observerPhong = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
    IObserver* observerBlinn = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "blinn-phong.frag");
    scene->setLight(new Light(vec3(0.0), vec3(0.385, 0.647, 0.812)), observerConstant);
	scene->addLightObserver(observerLambert);
	scene->addLightObserver(observerPhong);
	scene->addLightObserver(observerBlinn);
	scene->addCameraObserver(observerConstant);
    scene->addCameraObserver(observerLambert);
    scene->addCameraObserver(observerPhong);
    scene->addCameraObserver(observerBlinn);
    DrawableObject* drawableObject;


    vector<DrawableObject*> setterCollection;

    for (int i = 0; i < 4; i++)
    {
        modelSphere = new Model(sphere);


        switch (i)
        {
        default:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram *) observerConstant);
			drawableObject->addStaticTransformation(new Translation(vec3(0.0f, 1.0f, 0.0f)));
            break;
        case 1:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram*)observerLambert);
			drawableObject->addStaticTransformation(new Translation(vec3(1.0f, 0.0f, 0.0f)));
            break;
        case 2:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram*)observerPhong);
			drawableObject->addStaticTransformation(new Translation(vec3(0.0f, -1.0f, 0.0f)));
            break;
        case 3:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram*)observerBlinn);
			drawableObject->addStaticTransformation(new Translation(vec3(-1.0f, 0.0f, 0.0f)));
            break;
        }
		drawableObject->addStaticTransformation(new Scaling(vec3(0.5f, 0.5f, 0.5f)));
        scene->addObject(drawableObject);
    }
	return scene;
}

Scene* sceneTreesAndBushes()
{
    Scene* scene = new Scene();
    IObserver* observer = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
	scene->setLight(new Light(vec3(10.0f, 10.0f, 10.0f), vec3(1.0f, 1.0f, 0.0f)), observer);
    scene->addCameraObserver(observer);

    Model* model = new Model();


    DrawableObject* obj = new DrawableObject();

	model = new Model(plain);

	obj = new DrawableObject(model, (ShaderProgram*)observer);

	TransformGroup* trans = new TransformGroup();
	trans->addTransform(new Scaling(vec3(2.5f, 1.0f, 2.5f)));
	trans->addTransform(new Translation(vec3(1.0f, 0.0f, 1.0f)));

    obj->addStaticTransformation(trans);

	scene->addObject(obj);

    for (int i = 0; i < 100; i++)
    {

        model = new Model(tree);

        obj = new DrawableObject(model,(ShaderProgram*) observer);

        obj->addStaticTransformation(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

        float random = (float)(rand() % 20) / (float)100 + 0.1f;
        obj->addStaticTransformation(new Scaling(vec3(random, random, random)));

        scene->addObject(obj);
    }

    for (int i = 0; i < 100; i++)
    {
        model = new Model(bushes);

        obj = new DrawableObject(model, (ShaderProgram *) observer);

        obj->addStaticTransformation(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

        float random = (float)(rand() % 10) / (float)100 + 0.15f;
        obj->addStaticTransformation(new Scaling(vec3(random, random, random)));

        scene->addObject(obj);
    }

	return scene;
}



#include "Scene.h"

void Scene::render()
{
    for (auto entity : mWorldEntities)
    {
        entity->draw();
    }
}

void Scene::update(GLFWwindow* window, float deltaTime, Controls* controls)
{
    for (auto entity : mWorldEntities)
    {
        entity->update(window, deltaTime, controls);
    }
}


Scene* sceneDefault()
{
    vector<float>triangle = {
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    Scene* scene = new Scene();

    Model* modelTriangle = new Model(triangle);
    Model* modelSphere = new Model(sphere);

	ShaderProgram* shaderProgram = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");

    DrawableObject* entity = new DrawableObject(modelSphere, shaderProgram);

    entity->getTransformManager()->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));

    scene->addEntity(entity);

	Camera* camera = new Camera();
    camera->attach(shaderProgram);

    scene->addEntity(camera);

    Light* light = new Light(vec3(1.0f, 1.0f, 1.0f), 1.0f);

    light->setModel(modelSphere);
    light->addShaderProgram(shaderProgram);

    light->addStaticTransform(new Scaling(vec3(0.2f, 0.2f, 0.2f)));
    light->addStaticTransform(new Translation(vec3(1.5f, 1.5f, 1.5f)));

    light->addGlobalTransform(new Rotation(vec3(0.0f, 2.0f, 0.0f)));

    light->attach(shaderProgram);
    scene->addEntity(light);

    return scene;
}

/*
 Scene* sceneSpheres()
 {
     Scene* scene = new Scene();

     Model* modelSphere;
     ShaderProgram* shaderConstant = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
     ShaderProgram* shaderLambert = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "lambert.frag");
     ShaderProgram* shaderPhong = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
     ShaderProgram* shaderBlinn = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "blinn-phong.frag");
     scene->addLight(new Light(vec3(0.385, 0.647, 0.812), 1), (IObserver*) shaderConstant);
     scene->addLightObserver((IObserver*) shaderLambert);
     scene->addLightObserver((IObserver*) shaderPhong);
     scene->addLightObserver((IObserver*) shaderBlinn);

     scene->addCameraObserver((IObserver*) shaderConstant);
     scene->addCameraObserver((IObserver*) shaderLambert);
     scene->addCameraObserver((IObserver*) shaderPhong);
     scene->addCameraObserver((IObserver*) shaderBlinn);
     DrawableObject* drawableObject;
     vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);


     vector<DrawableObject*> setterCollection;

     for (int i = 0; i < 4; i++)
     {
         modelSphere = new Model(sphere);


         switch (i)
         {
         default:
             drawableObject = new DrawableObject(modelSphere, shaderConstant, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(0.0f, 1.0f, 0.0f)));
             break;
         case 1:
             drawableObject = new DrawableObject(modelSphere, shaderLambert, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(1.0f, 0.0f, 0.0f)));
             break;
         case 2:
             drawableObject = new DrawableObject(modelSphere, shaderPhong, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(0.0f, -1.0f, 0.0f)));
             break;
         case 3:
             drawableObject = new DrawableObject(modelSphere, shaderBlinn, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(-1.0f, 0.0f, 0.0f)));
             break;
         }
         drawableObject->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));
         scene->addObject(drawableObject);
     }
     return scene;
 }

 Scene* sceneTreesAndBushes()
 {
     Scene* scene = new Scene();
     ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
 	 scene->addLight(new Light(vec3(0.385, 0.647, 0.812), 1), shader);
     scene->addCameraObserver((IObserver*) shader);

     Model* model = new Model();

     vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);
     DrawableObject* obj = new DrawableObject();

 	model = new Model(plain);

 	obj = new DrawableObject(model, shader, objectColor);

     obj->addStaticTransform(new TransformComponent({ 
         new Scaling(vec3(2.5f, 1.0f, 2.5f)),
         new Translation(vec3(1.0f, 0.0f, 1.0f))
         }));

 	scene->addObject(obj);

     for (int i = 0; i < 100; i++)
     {

         model = new Model(tree);

         obj = new DrawableObject(model, shader, objectColor);

         obj->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

         float random = (float)(rand() % 20) / (float)100 + 0.1f;
         obj->addStaticTransform(new Scaling(vec3(random, random, random)));

         scene->addObject(obj);
     }

     for (int i = 0; i < 100; i++)
     {
         model = new Model(bushes);

         obj = new DrawableObject(model, shader, objectColor);

         obj->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

         float random = (float)(rand() % 10) / (float)100 + 0.15f;
         obj->addStaticTransform(new Scaling(vec3(random, random, random)));

         scene->addObject(obj);
     }

 	return scene;
 }

Scene* sceneSolarSystem()
{
	Scene* scene = new Scene();

	//ShaderProgram* sunShader = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
	ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "blinn-phong.frag");

    //scene->addCameraObserver((IObserver*)sunShader); --useless->constant shader
	scene->addCameraObserver((IObserver*)shader);


	scene->addLight(new Light(vec3(0.385, 0.647, 0.812), 1), (IObserver*)shader);
	//scene->addLightObserver((IObserver*)sunShader); -- useless -> constant shader

	Model* modelSphere = new Model(sphere);
    vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);
	DrawableObject* sun = new DrawableObject(modelSphere, shader, objectColor);
	scene->addObject(sun);

    float earthDistance = 4.0f;
	DrawableObject* earth = new DrawableObject(modelSphere, shader, objectColor);
    earth->addStaticTransform(new Translation(vec3(earthDistance, 0.0f, 0.0f)));
    earth->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));
	earth->addGlobalTransform(new Rotation(vec3(0.0f, 1.0f, 0.0f)));
	earth->addLocalTransform(new Rotation(vec3(0.0f, 3.0f, 0.0f)));
	scene->addObject(earth);


    DrawableObject* moon = new DrawableObject(modelSphere, shader, objectColor);
	moon->addParent(earth->getTransformManager());
    moon->addLocalTransform(new TransformComponent({
        new Translation(vec3(-1.0f, 0.0f, 0.0f)),
        new Rotation(vec3(0.0f, 1.0f, 0.0f)),
        new Translation(vec3(1.0f, 0.0f, 0.0f))
    }));
    
    moon->addStaticTransform(new Translation(vec3(earthDistance + 1, 0.0f, 0.0f)));
    moon->addStaticTransform(new Scaling(vec3(0.2f, 0.2f, 0.2f)));
	scene->addObject(moon);

	return scene;
}*/



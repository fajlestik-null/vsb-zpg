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

    DrawableObject* entity = new DrawableObject(modelSphere, shaderProgram, vec3(1,0,0));

    entity->getTransformManager()->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));

    scene->addEntity(entity);

	Camera* camera = new Camera();
    camera->attach(shaderProgram);

    scene->addEntity(camera);

    Light* light = new Light(vec3(1.0f, 1.0f, 1.0f), 1.0f);

    //light->setModel(modelTriangle);
    //light->addShaderProgram(shaderProgram);

    light->addStaticTransform(new Scaling(vec3(0.2f, 0.2f, 0.2f)));
    light->addStaticTransform(new Translation(vec3(1.5f, 1.5f, 1.5f)));

    //light->addGlobalTransform(new Rotation(vec3(0.0f, 2.0f, 0.0f)));

    light->attach(shaderProgram);
    scene->addEntity(light);

    return scene;
}


 Scene* sceneSpheres()
 {
     Scene* scene = new Scene();

     Model* modelSphere;
     ShaderProgram* shaderConstant = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
     ShaderProgram* shaderLambert = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "lambert.frag");
     ShaderProgram* shaderPhong = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
     ShaderProgram* shaderBlinn = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "blinn-phong.frag");
     
     DrawableObject* drawableObject;
     vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);

     for (int i = 0; i < 4; i++)
     {
         modelSphere = new Model(sphere);


         switch (i)
         {
         default:
             drawableObject = new DrawableObject(modelSphere, shaderConstant, vec3(1,0,0));
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
         scene->addEntity(drawableObject);
     }

     
	 Light* light = new Light(vec3(0.385, 0.647, 0.812), 1);
     light->attach(shaderConstant);
     light->attach(shaderLambert);
     light->attach(shaderPhong);
     light->attach(shaderBlinn);
	 scene->addEntity(light);

     Camera* camera = new Camera();
     camera->attach(shaderConstant);
     camera->attach(shaderLambert);
     camera->attach(shaderPhong);
     camera->attach(shaderBlinn);
	 scene->addEntity(camera);


     return scene;
 }

 Scene* sceneTreesAndBushes()
 {
     Scene* scene = new Scene();
     ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");

     Model* model = new Model();

     DrawableObject* entity = new DrawableObject();

 	model = new Model(plain);

 	entity = new DrawableObject(model, shader, vec3(0.5f, 0.2f, 0.2f));

     entity->addStaticTransform(new TransformComponent({ 
         new Scaling(vec3(2.5f, 1.0f, 2.5f)),
         new Translation(vec3(1.0f, 0.0f, 1.0f))
         }));

 	scene->addEntity(entity);

     for (int i = 0; i < 100; i++)
     {

         model = new Model(tree);
         entity = new DrawableObject(model, shader, vec3(0.42f, 0.126f, 0.25f));

         entity->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

         float random = (float)(rand() % 20) / (float)100 + 0.1f;
         entity->addStaticTransform(new Scaling(vec3(random, random, random)));

         scene->addEntity(entity);
     }

     for (int i = 0; i < 100; i++)
     {
         model = new Model(bushes);

         entity = new DrawableObject(model, shader, vec3(0.124f, 0.252f, 0.0f));

         entity->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

         float random = (float)(rand() % 10) / (float)100 + 0.15f;
         entity->addStaticTransform(new Scaling(vec3(random, random, random)));

         scene->addEntity(entity);
     }

     Light* light = new Light(vec3(0.385, 0.647, 0.812), 1);
     light->addStaticTransform(new Translation(vec3(10.0f, 10.0f, 10.0f)));
     //light->setModel(new Model(sphere));
     //light->addShaderProgram(shader);
     light->attach(shader);
     scene->addEntity(light);

     Camera* camera = new Camera();
     camera->attach(shader);
     scene->addEntity(camera);

 	return scene;
 }

Scene* sceneSolarSystem()
{
	Scene* scene = new Scene();

	ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");

	Model* modelSphere = new Model(sphere);
	DrawableObject* sun = new DrawableObject(modelSphere, shader, vec3(1.0f,0.65f,0.0f));
	scene->addEntity(sun);

    float earthDistance = 4.0f;
	DrawableObject* earth = new DrawableObject(modelSphere, shader, vec3(0.5f,0.38f,0.26f));
    earth->addStaticTransform(new Translation(vec3(earthDistance, 0.0f, 0.0f)));
    earth->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));
	earth->addGlobalTransform(new Rotation(vec3(0.0f, 1.0f, 0.0f)));
	earth->addLocalTransform(new Rotation(vec3(0.0f, 3.0f, 0.0f)));
	scene->addEntity(earth);


    DrawableObject* moon = new DrawableObject(modelSphere, shader, vec3(0.96f,0.95f, 0.84f));
	moon->addParent(earth->getTransformManager());
    moon->addLocalTransform(new TransformComponent({
        new Translation(vec3(-1.0f, 0.0f, 0.0f)),
        new Rotation(vec3(0.0f, 1.0f, 0.0f)),
        new Translation(vec3(1.0f, 0.0f, 0.0f))
    }));
    
    moon->addStaticTransform(new Translation(vec3(earthDistance + 1, 0.0f, 0.0f)));
    moon->addStaticTransform(new Scaling(vec3(0.2f, 0.2f, 0.2f)));
	scene->addEntity(moon);

    Camera* camera = new Camera();
    camera->attach(shader);
    scene->addEntity(camera);


    Light* light = new Light(vec3(0.385, 0.647, 0.812), 1);
    light->addStaticTransform(new Translation(vec3(10.0f, 5.0f, 5.0f)));
    light->attach(shader);
    scene->addEntity(light);

	return scene;
}



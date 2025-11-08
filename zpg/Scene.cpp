#include "Scene.h"


void Scene::render()
{
    for (auto entity : mWorldEntities)
    {
        entity->draw();
    }
}

Scene::~Scene()
{
    for(auto we : mWorldEntities)
    {
        delete we;
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

    DrawableObject* entity = new DrawableObject(modelTriangle, shaderProgram, vec3(1,0,0));

    entity->getTransformManager()->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));

    scene->addEntity(entity);

	Camera* camera = new Camera();
    camera->attach(shaderProgram);

    scene->addEntity(camera);

    Light* light = new Light(LightType::POINT,vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f);

    light->setModel(modelSphere);
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

     Model* model;
     //ShaderProgram* shaderConstant = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
     //ShaderProgram* shaderLambert = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "lambert.frag");
     ShaderProgram* shaderPhong = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
     ShaderProgram* reflector = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "reflector.frag");
     
     DrawableObject* drawableObject;
     vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);

     model = new Model(plain);

     drawableObject = new DrawableObject(model, reflector, vec3(1, 0, 0));
     //

     drawableObject->addStaticTransform(new Translation(vec3(0, 0, -1)));
     drawableObject->addStaticTransform(new Rotation(vec3(0, 90, 90)));
     drawableObject->addStaticTransform(new Scaling(vec3(2.5f, 1.0f, 2.5f)));

     scene->addEntity(drawableObject);


     for (int i = 0; i < 4; i++)
     {
         model = new Model(sphere);


         switch (i)
         {
         default:
             //drawableObject = new DrawableObject(model, shaderPhong, objectColor);
             drawableObject = new DrawableObject(model, reflector, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(0.0f, 1.0f, 0.0f)));
             break;
         case 1:
             //drawableObject = new DrawableObject(model, shaderPhong, objectColor);
             drawableObject = new DrawableObject(model, reflector, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(1.0f, 0.0f, 0.0f)));
             break;
         case 2:
             //drawableObject = new DrawableObject(model, shaderPhong, objectColor);
             drawableObject = new DrawableObject(model, reflector, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(0.0f, -1.0f, 0.0f)));
             break;
         case 3:
             //drawableObject = new DrawableObject(model, shaderPhong, objectColor);
             drawableObject = new DrawableObject(model, reflector, objectColor);
             drawableObject->addStaticTransform(new Translation(vec3(-1.0f, 0.0f, 0.0f)));
             break;
         }
         drawableObject->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));
         scene->addEntity(drawableObject);
     }

     
	 Light* light = new Light(LightType::POINT,vec3(0.385, 0.647, 0.812), 1.0f, 1.0f);
     //light->attach(shaderConstant);
     //light->attach(shaderLambert);
     light->attach(reflector);
     //light->attach(shaderBlinn);
	 scene->addEntity(light);

     Camera* camera = new Camera();
     //camera->attach(shaderConstant);
     //camera->attach(shaderLambert);
     camera->attach(reflector);
     //camera->attach(shaderBlinn);
	 scene->addEntity(camera);


     return scene;
 }

 Scene* sceneTreesAndBushes()
 {
     Scene* scene = new Scene();
     ResourceManager& resourceManager = ResourceManager::getInstance();

     ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "generalLight.frag");
     ShaderProgram* shaderFirefly = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");

	 Texture * grass = resourceManager.loadTexture("./Res/grass.png");
	 Model* ground = resourceManager.loadModel("./Res/square.obj");

     ground->setTexture(grass);


     DrawableObject* entity = new DrawableObject();

 	entity = new DrawableObject(ground, shader, vec3(0.5f, 0.2f, 0.2f));


    //modelPlain->setTexture(resourceManager.loadTexture("./Res/grass.png"));
    
     entity->addStaticTransform(new TransformComponent({ 
         new Scaling(vec3(2.5f, 1.0f, 2.5f)),
         new Translation(vec3(1.0f, 0.0f, 1.0f))
         }));

 	scene->addEntity(entity);

    Model* model = new Model();

     model = new Model(tree);
     for (int i = 0; i < 50; i++)
     {

         entity = new DrawableObject(model, shader, vec3(0.42f, 0.126f, 0.25f));

         entity->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

         float random = (float)(rand() % 20) / (float)100 + 0.1f;
         entity->addStaticTransform(new Scaling(vec3(random, random, random)));

        scene->addEntity(entity);
     }

     model = new Model(bushes);
     for (int i = 0; i < 100; i++)
     {

         entity = new DrawableObject(model, shader, vec3(0.124f, 0.252f, 0.0f));

         entity->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

         float random = (float)(rand() % 10) / (float)100 + 0.15f;
         entity->addStaticTransform(new Scaling(vec3(random, random, random)));

         scene->addEntity(entity);
     }
     
     model = new Model(sphere);

     Light* light;


    for (int i = 0; i < 5; i++)
    {
        light = new Light(LightType::POINT, vec3(0.886f, 1.0f, 0.6f), 1.0f, 0.2f);
        light->setModel(model);
        light->addShaderProgram(shaderFirefly);
        light->addStaticTransform(new Scaling(vec3(0.002f, 0.002f, 0.002f)));

        light->addStaticTransform(new Translation(vec3(2.0f, 0.5f, 4.0f)));

        light->addLocalTransform(new TransformTimer({{0.5, new Rotation(vec3(0.0f, -(40.0f), 0.0f), vec3(0.0f, (40.0f), 0.0f))}, {0, new Translation(vec3(0.005f, 0.0f, 0.0f))}})); //{min},{max}
        light->attach(shader);
		light->attach(shaderFirefly);
        scene->addEntity(light);
    }

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(shaderFirefly);
    scene->addEntity(camera);

    light = new Light(LightType::REFLECTOR, vec3(0.90f, 0.95f, 1.0f), 2.0f, 0.75f);
    //light->setModel(model);
    //light->addShaderProgram(shader);
    light->addParent(camera->getTransformManager());
    light->setActive(true);

    light->attach(shader);
    scene->addEntity(light);


 	return scene;
 }

Scene* sceneSolarSystem()
{
	Scene* scene = new Scene();

    ResourceManager& resourceManager = ResourceManager::getInstance();


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


    Light* light = new Light(LightType::POINT,vec3(0.385, 0.647, 0.812), 1.0f, 1.0f);
    light->addStaticTransform(new Translation(vec3(0.0f, 0.0f, 0.0f)));
    light->setModel(modelSphere);
    light->addShaderProgram(shader);
    light->attach(shader);
    scene->addEntity(light);

	return scene;
}

Scene* sceneTesting()
{
	Scene* scene = new Scene();

	ResourceManager& resourceManager = ResourceManager::getInstance();

    Model* model = resourceManager.loadModel("./Res/cube.obj");
    ShaderProgram* shader = resourceManager.loadShaderProgram("lambert.vert", "generalLight.frag");
    Texture* wood = resourceManager.loadTexture("./Res/wooden_fence.png");

	DrawableObject* cube = new DrawableObject(model, shader, vec3(0.8f, 0.7f, 0.6f));

	model->setTexture(wood);

	scene->addEntity(cube);

    Model* cubeM = resourceManager.loadModel("./Res/cube.obj");

	DrawableObject* cube_en = new DrawableObject(cubeM, shader, vec3(0.8f, 0.7f, 0.6f));

	cube_en->addStaticTransform(new Translation(vec3(1.5f, 0.0f, 0.0f)));

	scene->addEntity(cube_en);


    /*Model* shrek = resourceManager.loadModel("./Res/shrek.obj");
	Model* fiona = resourceManager.loadModel("./Res/fiona.obj");
    ShaderProgram* shader = resourceManager.loadShaderProgram("lambert.vert", "generalLight.frag");
	Texture* shrek_tx = resourceManager.loadTexture("./Res/shrek.png");


    DrawableObject* shrek_en = new DrawableObject(shrek, shader, vec3(0.0f, 0.1f, 0.0f));
    shrek->setTexture(shrek_tx);

    scene->addEntity(shrek_en);
    

    DrawableObject* fiona_en = new DrawableObject(fiona, shader, vec3(0.0f, 0.1f, 0.0f));

    Texture* fiona_tx = resourceManager.loadTexture("./Res/fiona.png");
    fiona->setTexture(fiona_tx);

	fiona_en->addStaticTransform(new Translation(vec3(1.0f, 0.1f, 0.1f)));

    scene->addEntity(fiona_en);*/

	Camera* camera = new Camera();
	camera->attach(shader);
	scene->addEntity(camera);

	Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 0.5f, 1.0f);
	light->addStaticTransform(new Translation(vec3(2.0f, 2.0f, 2.0f)));
	light->attach(shader);
	scene->addEntity(light);

	return scene;
}





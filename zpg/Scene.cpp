#include "Scene.h"

Scene::~Scene()
{
    for(auto we : mWorldEntities)
    {
        delete we;
    }
}

void Scene::render(GLFWwindow* window, float deltaTime, Controls* controls)
{
    if (mSkyBox != nullptr)
    {
        mSkyBox->update(window, deltaTime, controls);
        mSkyBox->draw();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

	setSelectedEntity(controls);
	insertEntity(controls);
	removeEntity(controls);

    for (auto entity : mWorldEntities)
    {
        glStencilFunc(GL_ALWAYS, entity->getStencilIndex(), 0xFF);
        entity->update(window, deltaTime, controls);
		transformEntity(controls); //needs to be after update to get correct position from controls
        entity->draw();
    }

    if (mActiveCamera)
		mActiveCamera->notifyObservers();
}

void Scene::setSelectedEntity(Controls* controls)
{
    if (controls->isMouseButtonTriggered(GLFW_MOUSE_BUTTON_LEFT))
    {
        for (auto entity : mWorldEntities)
        {
            if (entity->getStencilIndex() == controls->getStencilIndex() && controls->getStencilIndex() != 0)
            {
                mEntitityToHandle = entity;
				cout << "Selected entity with stencil index: " << controls->getStencilIndex() << endl;
                break;
            }
		}
    }

}

void Scene::removeEntity(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_DELETE) && mEntitityToHandle != nullptr)
    {
        auto it = find(mWorldEntities.begin(), mWorldEntities.end(), mEntitityToHandle);
        if (it != mWorldEntities.end())
        {
            delete *it;
            mWorldEntities.erase(it);
            mEntitityToHandle = nullptr;
        }
	}
}

void Scene::insertEntity(Controls* controls)
{
    if (controls->isKeyTriggered(GLFW_KEY_I) && mEntitityToHandle != nullptr)
    {
        WorldEntity* duplicate = mEntitityToHandle->getCopy();
        duplicate->addStaticTransform(new Translation(controls->getPosition()));
        this->addEntity(duplicate);
    }
}

void Scene::transformEntity(Controls* controls)
{
    if (mEntitityToHandle != nullptr)
    {
        if (controls->isKeyTriggered(GLFW_KEY_T))
        {
            vec3 mousePosition = controls->getPosition();
            vec3 currentPosition = vec3(mEntitityToHandle->getTransformManager()->getFinalMatrix()[3]);
            vec3 direction = mousePosition - currentPosition;
            mEntitityToHandle->addStaticTransform(new Translation(direction));
        }
        if (controls->isKeyPressed(GLFW_KEY_R))
        {
            if (controls->isKeyPressed(GLFW_KEY_X))
            {
                mEntitityToHandle->addStaticTransform(new Rotation(vec3(1.0f, 0.0f, 0.0f)));
            }
            if (controls->isKeyPressed(GLFW_KEY_Y))
            {
                mEntitityToHandle->addStaticTransform(new Rotation(vec3(0.0f, 0.0f, 1.0f)));
            }
            if (controls->isKeyPressed(GLFW_KEY_Z))
            {
                mEntitityToHandle->addStaticTransform(new Rotation(vec3(0.0f, 1.0f, 0.0f)));
            }
        }
        //scalling
        if(controls->isKeyPressed(GLFW_KEY_UP))
        mEntitityToHandle->addStaticTransform(new Scaling(vec3(1.01f, 1.01f, 1.01f)));
        if (controls->isKeyPressed(GLFW_KEY_DOWN))
        mEntitityToHandle->addStaticTransform(new Scaling(vec3(0.99f, 0.99f, 0.99f)));

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

	ShaderProgram* shaderProgram = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "generalLight.frag");

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

     Model* model = new Model();

     ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "generalLight.frag");
     ShaderProgram* shaderTexture = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "generalLight.frag");
     ShaderProgram* shaderFirefly = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");

	 Texture * grass = resourceManager.loadTexture("./Res/grass.png");
	 model = resourceManager.loadModel("./Res/square.obj");


     DrawableObject* ground = new DrawableObject();

     ground = new DrawableObject(model, shaderTexture, vec3(0.5f, 0.2f, 0.2f));

	 ground->setTexture(grass);

    //modelPlain->setTexture(resourceManager.loadTexture("./Res/grass.png"));
    
     ground->addStaticTransform(new TransformComponent({ 
         new Scaling(vec3(2.5f, 1.0f, 2.5f)),
         new Translation(vec3(1.0f, 0.0f, 1.0f))
         }));

 	scene->addEntity(ground);

    DrawableObject* entity = new DrawableObject();

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

     model = resourceManager.loadModel("./Res/shrek.obj");
     Texture* shrek_tx = resourceManager.loadTexture("./Res/shrek.png");


     entity = new DrawableObject(model, shaderTexture, vec3(0.0f, 0.1f, 0.0f));
     entity->setTexture(shrek_tx);

     entity->addStaticTransform(new Translation(vec3(2.0f, 0.0f, 1.9f)));
     entity->addStaticTransform(new Scaling(vec3(0.1f, 0.1f, 0.1f)));

     scene->addEntity(entity);


     model = resourceManager.loadModel("./Res/fiona.obj");
     entity = new DrawableObject(model, shaderTexture, vec3(0.0f, 0.1f, 0.0f));

     Texture* fiona_tx = resourceManager.loadTexture("./Res/fiona.png");
     entity->setTexture(fiona_tx);

     entity->addStaticTransform(new Translation(vec3(2.0f, 0.0f, 2.1f)));
     entity->addStaticTransform(new Scaling(vec3(0.1f, 0.1f, 0.1f)));

     scene->addEntity(entity);


     model = resourceManager.loadModel("./Res/toiled.obj");
     entity = new DrawableObject(model, shaderTexture, vec3(0.0f, 0.1f, 0.0f));

     Texture* toilet_tx = resourceManager.loadTexture("./Res/toiled.jpg");
     entity->setTexture(toilet_tx);

	 entity->addStaticTransform(new Translation(vec3(2.0f, 0.0f, 2.0f)));
	 entity->addStaticTransform(new Scaling(vec3(0.1f, 0.1f, 0.1f)));

     scene->addEntity(entity);
     
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
		light->attach(shaderTexture);
		light->attach(shaderFirefly);
        scene->addEntity(light);
    }

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(shaderFirefly);
	camera->attach(shaderTexture);
    scene->addEntity(camera);

    light = new Light(LightType::REFLECTOR, vec3(0.90f, 0.95f, 1.0f), 2.0f, 0.75f);
    //light->setModel(model);
    //light->addShaderProgram(shader);
    light->addParent(camera->getTransformManager());
    light->setActive(true);
    light->attach(shader);
	light->attach(shaderTexture);
    scene->addEntity(light);

    light = new Light(LightType::AMBIENT, vec3(0.90f, 0.95f, 1.0f),0.01f, 0.75f);
    //light->setModel(model);
    //light->addShaderProgram(shader);
    light->attach(shader);
    light->attach(shaderTexture);
    scene->addEntity(light);

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

    auto textureSkyBox = resourceManager.loadCubeMap("mySkyBox",{ "./Res/posx.jpg",
                                        "./Res/negx.jpg",
                                        "./Res/posy.jpg",
                                        "./Res/negy.jpg",
                                        "./Res/posz.jpg",
                                        "./Res/negz.jpg" });


	Model* skybox = resourceManager.loadModel("./Res/skybox.obj");
    ShaderProgram * shaderSky = resourceManager.loadShaderProgram("skyBox.vert", "skyBox.frag");

    DrawableObject* sky = new DrawableObject(skybox, shaderSky, vec3(0.8f, 0.7f, 0.6f));


    //  texture_skybox->load_from_files(paths);
    sky->setTexture(textureSkyBox);

    //obj_2->add_static_transform(new Scale(vec3(20.0f, 20.0f, 20.0f)));
    //obj_2->add_static_transform(new Transfer(vec3(10.0f, 1.0f, 10.0f)));
    scene->setSkyBox(sky);

    Model* model = resourceManager.loadModel("./Res/cube.obj");
    ShaderProgram* shader = resourceManager.loadShaderProgram("lambert.vert", "generalLight.frag");
    Texture* wood = resourceManager.loadTexture("./Res/wooden_fence.png");

	DrawableObject* cube = new DrawableObject(model, shader, vec3(0.8f, 0.7f, 0.6f));

	cube->setTexture(wood);

	scene->addEntity(cube);

    Model* cubeM = resourceManager.loadModel("./Res/cube.obj");

	DrawableObject* cube_en = new DrawableObject(cubeM, shader, vec3(0.8f, 0.7f, 0.6f));

	cube_en->addStaticTransform(new Translation(vec3(1.5f, 0.0f, 0.0f)));

	scene->addEntity(cube_en);


    Model* shrek = resourceManager.loadModel("./Res/shrek.obj");
	Model* fiona = resourceManager.loadModel("./Res/fiona.obj");
	Texture* shrek_tx = resourceManager.loadTexture("./Res/shrek.png");


    DrawableObject* shrek_en = new DrawableObject(shrek, shader, vec3(0.0f, 0.1f, 0.0f));
    shrek_en->setTexture(shrek_tx);
    shrek_en->setStencilIndex(22);
    scene->addEntity(shrek_en);
    

    DrawableObject* fiona_en = new DrawableObject(fiona, shader, vec3(0.0f, 0.1f, 0.0f));

    Texture* fiona_tx = resourceManager.loadTexture("./Res/fiona.png");
    fiona_en->setTexture(fiona_tx);

	fiona_en->addStaticTransform(new Translation(vec3(1.0f, 0.1f, 0.1f)));

    scene->addEntity(fiona_en);

	ShaderProgram *shaderGrass = resourceManager.loadShaderProgram("grass.vert", "generalLight.frag");

	DrawableObject* terain = new DrawableObject(resourceManager.loadModel("./Res/teren.obj"), shaderGrass, vec3(0.0f, 0.1f, 0.0f));

	terain->setTexture(resourceManager.loadTexture("./Res/grass.png"));
    
	scene->addEntity(terain);

	Camera* camera = new Camera();
	camera->attach(shader);
    camera->attach(shaderSky);
	camera->attach(shaderGrass);
	scene->addEntity(camera);

	sky->addParent(camera->getTransformManager());
	sky->getTransformManager()->setInheritOnlyTranslation(InheritTransformation::TRANSLATION);

	Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 0.5f, 1.0f);
	light->addStaticTransform(new Translation(vec3(2.0f, 2.0f, 2.0f)));
	light->attach(shader);
	light->attach(shaderSky);
	light->attach(shaderGrass);
	scene->addEntity(light);

	return scene;
}





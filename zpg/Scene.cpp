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
    transformEntity(controls); //needs to be after update to get correct position from controls

    for (auto entity : mWorldEntities)
    {   
        glStencilFunc(GL_ALWAYS, entity->getStencilIndex(), 0xFF);
        entity->update(window, deltaTime, controls);
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
        //duplicate->nullifyTranslation();
        duplicate->addStaticTransform(new Translation(controls->getPosition()));
		this->mWorldEntities.insert(mWorldEntities.begin(), duplicate);
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
     for (int i = 0; i < 50; i++)
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

    ResourceManager& resourceManager = ResourceManager::getInstance();

    auto textureSkyBox = resourceManager.loadCubeMap("mySkyBox", { "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg" });


    Model* skybox = resourceManager.loadModel("./Res/skybox.obj");
    ShaderProgram* shaderSky = resourceManager.loadShaderProgram("skyBox.vert", "skyBox.frag");

    DrawableObject* sky = new DrawableObject(skybox, shaderSky, vec3(1.0f, 1.0f, 1.0f));

    sky->setTexture(textureSkyBox);

	scene->setSkyBox(sky);


    ShaderProgram* shader = resourceManager.loadShaderProgram("lambert.vert", "generalLight.frag");
   //ShaderProgram* shaderT = resourceManager.loadShaderProgram("constant.vert", "constant.frag");


    Model* model = resourceManager.loadModel("./Res/planet.obj");

	Texture* texture = resourceManager.loadTexture("./Res/sun.jpg");

	float offset = -20.0f;
	float slowCoefficient = 10.0f;
    float earthGlobalRotation = 1.0f/slowCoefficient;
    float earthSelftRotation = 366.256f/slowCoefficient;

	DrawableObject* sun = new DrawableObject(model, shader);
	float sunSize = 15.0f;
	sun->addStaticTransform(new Translation(vec3(0.0f, offset, 0.0f)));
	sun->addStaticTransform(new Scaling(vec3(sunSize, sunSize, sunSize)));
    sun->setTexture(texture);
	scene->addEntity(sun);

    DrawableObject* mercury = new DrawableObject(model, shader);
	mercury->addStaticTransform(new Translation(vec3(sunSize + 17.96, offset/2, 0.0f)));
	mercury->addStaticTransform(new Scaling(vec3(0.38f, 0.38f, 0.38f)));
	mercury->addGlobalTransform(new Rotation(vec3(0.0f, 4.15f*earthGlobalRotation, 0.0f)));
    //tilt
	//mercury->addLocalTransform(new Rotation(vec3(0.0f, 0.0f, 0.0034f)));
	mercury->addLocalTransform(new Rotation(vec3(0.0f, 0.016f*earthSelftRotation, 0.0f)));
	texture = resourceManager.loadTexture("./Res/mercury.jpg");
	mercury->setTexture(texture);
    scene->addEntity(mercury);

    DrawableObject* venus = new DrawableObject(model, shader);
	venus->addStaticTransform(new Translation(vec3(sunSize + 33.16f, offset/2, 0.0f)));
	venus->addStaticTransform(new Scaling(vec3(0.95f, 0.95f, 0.95f)));
	venus->addGlobalTransform(new Rotation(vec3(0.0f, 3.07f*earthGlobalRotation, 0.0f)));
	//tilt
	//venus->addLocalTransform(new Rotation(vec3(0.0f, 0.0f, 177.4f)));
	venus->addLocalTransform(new Rotation(vec3(0.0f, 0.004*earthSelftRotation, 0.0f)));
	texture = resourceManager.loadTexture("./Res/venus.jpg");
	venus->setTexture(texture);
	scene->addEntity(venus);

	DrawableObject* earth = new DrawableObject(model, shader);
    earth->addStaticTransform(new Translation(vec3(sunSize + 46.05f, offset/2, 0.0f)));
	earth->addGlobalTransform(new Rotation(vec3(0.0f, earthGlobalRotation, 0.0f)));
	//tilt
	//earth->addLocalTransform(new Rotation(vec3(0.0f, 0.0f, 23.44f)));
	earth->addStaticTransform(new Scaling(vec3(0.95f, 0.95f * 0.996f, 0.95f))); //a bit elliptical - in testing phase
	earth->addLocalTransform(new Rotation(vec3(0.0f, earthSelftRotation, 0.0f)));
	texture = resourceManager.loadTexture("./Res/earth.jpg");
	earth->setTexture(texture);
	scene->addEntity(earth);


    DrawableObject* moon = new DrawableObject(model, shader);
	moon->addParent(earth->getTransformManager());
    //tilt
	//moon->addLocalTransform(new Rotation(vec3(0.0f, 0.0f, 6.68f)));
	// moon orbit
    moon->addLocalTransform(new Rotation(vec3(0.0f, (earthSelftRotation / 27.3f) / slowCoefficient, 0.0f)));
    // orbit radius
    moon->addStaticTransform(new Translation(vec3(2.8f, 0.0f, 0.0f)));
    // moon size
    moon->addStaticTransform(new Scaling(vec3(0.27f)));

	texture = resourceManager.loadTexture("./Res/moon.jpg");
	moon->setTexture(texture);
	scene->addEntity(moon);

    DrawableObject* mars = new DrawableObject(model, shader);
	mars->addStaticTransform(new Translation(vec3(sunSize + 70.0f, offset/2, 0.0f)));
	mars->addStaticTransform(new Scaling(vec3(0.53f, 0.53f * 0.994f, 0.53f))); //a bit elliptical - in testing phase
	mars->addGlobalTransform(new Rotation(vec3(0.0f, 0.81f*earthGlobalRotation, 0.0f)));
	//tilt
	//mars->addLocalTransform(new Rotation(vec3(0.0f, 0.0f, 25.19f)));
	mars->addLocalTransform(new Rotation(vec3(0.0f, 0.97*earthSelftRotation, 0.0f)));
	texture = resourceManager.loadTexture("./Res/mars.jpg");
	mars->setTexture(texture);
	scene->addEntity(mars);


   Light* light = new Light(LightType::POINT,vec3(0.385, 0.647, 0.812), 1.0f, 1.0f);
	light->addParent(sun->getTransformManager());
    light->attach(shaderSky);
    light->attach(shader);
	//light->attach(shaderT);
    scene->addEntity(light);

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(shaderSky);
    //camera->attach(shaderT);
    scene->addEntity(camera);

    sky->addParent(camera->getTransformManager());
    sky->getTransformManager()->setInheritOnlyTransform(InheritTransformation::TRANSLATION);

	return scene;
}

Scene* sceneTesting()
{
	Scene* scene = new Scene();

	ResourceManager& resourceManager = ResourceManager::getInstance();

   auto textureSkyBox = resourceManager.loadCubeMap("mySkyBox", {"./Res/starfield1.jpg",
                                        "./Res/starfield1.jpg",
                                        "./Res/starfield1.jpg",
                                        "./Res/starfield1.jpg",
                                        "./Res/starfield1.jpg",
                                        "./Res/starfield1.jpg" });


	Model* skybox = resourceManager.loadModel("./Res/skybox.obj");
    ShaderProgram * shaderSky = resourceManager.loadShaderProgram("skyBox.vert", "skyBox.frag");

    DrawableObject* sky = new DrawableObject(skybox, shaderSky, vec3(0.8f, 0.7f, 0.6f));


    sky->setTexture(textureSkyBox);

    //obj_2->add_static_transform(new Scale(vec3(20.0f, 20.0f, 20.0f)));
    //obj_2->add_static_transform(new Transfer(vec3(10.0f, 1.0f, 10.0f)));
    scene->setSkyBox(sky);
    

    Model* model = resourceManager.loadModel("./Res/cube.obj");
    ShaderProgram* shader = resourceManager.loadShaderProgram("lambert.vert", "generalLight.frag");
    Texture* wood = resourceManager.loadTexture("./Res/wooden_fence.png");

	DrawableObject* cube = new DrawableObject(model, shader, vec3(0.8f, 0.7f, 0.6f));

	cube->setTexture(wood);

    cube->addStaticTransform(new Custom(mat4(1.0, 0.0, 0.0, 0.0,
                                              0.0, 1.0, 0.0, 0.0,
                                              0.0, 0.0, 1.0, 0.0,
                                              0.0, 0.0, 0.0, 20.0)));

	scene->addEntity(cube);

    Model* cubeM = resourceManager.loadModel("./Res/cube.obj");

	DrawableObject* cube_en = new DrawableObject(cubeM, shader, vec3(0.8f, 0.7f, 0.6f));

	cube_en->addStaticTransform(new Translation(vec3(1.5f, 0.0f, 0.0f)));
    cube_en->addStaticTransform(new Custom(mat4(1.0, 0.0, 0.0, 0.0,
                                                0.0, 1.0, 0.0, 0.0,
                                                0.0, 0.0, 1.0, 0.0,
                                                0.0, 0.0, 0.0, 20.0)));

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

	fiona_en->addStaticTransform(new Translation(vec3(1.0f, 0.0f, 0.0f)));

    scene->addEntity(fiona_en);

	ShaderProgram *shaderGrass = resourceManager.loadShaderProgram("grass.vert", "generalLight.frag");

	DrawableObject* terain = new DrawableObject(resourceManager.loadModel("./Res/teren.obj"), shaderGrass, vec3(0.0f, 0.1f, 0.0f));

	terain->setTexture(resourceManager.loadTexture("./Res/grass.png"));
    
	scene->addEntity(terain);

	model = resourceManager.loadModel("./Res/beaver.obj");

    DrawableObject* beaver = new DrawableObject(model, shader, vec3(0.0f, 0.1f, 0.0f));

    Texture* beaver_tx = resourceManager.loadTexture("./Res/beaver.jpg");
    beaver->setTexture(beaver_tx);

    beaver->addStaticTransform(new Translation(vec3(5.0f, 0.0f, 3.0f)));
	beaver->addStaticTransform(new Scaling(vec3(0.1f, 0.1f, 0.1f)));
	beaver->addStaticTransform(new Rotation(vec3(-90.0f, 0.0f, 0.0f)));

	beaver->setStencilIndex(1);

    scene->addEntity(beaver);

	Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 0.5f, 1.0f);
	light->addStaticTransform(new Translation(vec3(2.0f, 2.0f, 2.0f)));
	light->attach(shader);
	light->attach(shaderSky);
	light->attach(shaderGrass);
    scene->addEntity(light);

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(shaderSky);
    camera->attach(shaderGrass);
    scene->addEntity(camera);

    sky->addParent(camera->getTransformManager());
    sky->getTransformManager()->setInheritOnlyTransform(InheritTransformation::TRANSLATION);

	return scene;
}

/*
// Helper function needed for the random Z component generation (assuming you don't have it globally)
float randomFloat(float minVal, float maxVal)
{
    return minVal + static_cast<float>(rand()) / RAND_MAX * (maxVal - minVal);
}

Scene* sceneWhacAMole()
{

    Scene* scene = new Scene();
    ResourceManager& rm = ResourceManager::getInstance();

    // --- 1. Resources ---
    ShaderProgram* shader = rm.loadShaderProgram("lambert.vert", "generalLight.frag");
    Model* beaverModel = rm.loadModel("./Res/beaver.obj");
    Texture* moleTexture = rm.loadTexture("./Res/beaver.jpg");

    // --- 2. Static World Elements (Ground, Camera, Light) ---
    // (Setup omitted for brevity)

    // --- 3. Define Global Roaming Boundaries and Path ---
    const float GLOBAL_BOUND = 50.0f; // Roam within +/- 50 units
    const float ROAMING_HEIGHT_Y = 1.0f; // Fixed height (Y is vertical)
    const float ROAMING_SPEED = 0.00005f; // Very slow speed to cover the distance

    const int PATH_SEGMENTS = 20; // Number of points in the path

    // Define a single, long zig-zag path across the terrain
    std::vector<glm::vec3> global_roaming_path;

    for (int i = 0; i <= PATH_SEGMENTS; ++i)
    {
        float x = (static_cast<float>(i) / PATH_SEGMENTS) * (GLOBAL_BOUND * 2.0f) - GLOBAL_BOUND;

        // Use a sin wave or random Z component to create a zig-zag pattern
        float z;
        if (i % 2 == 0) {
            z = randomFloat(-GLOBAL_BOUND, GLOBAL_BOUND); // Random Z
        }
        else {
            z = (static_cast<float>(i) / PATH_SEGMENTS) * (GLOBAL_BOUND * 2.0f) - GLOBAL_BOUND; // Incremental Z
        }

        // The path points are in world space (since moles are now scene children)
        global_roaming_path.push_back(glm::vec3(x, ROAMING_HEIGHT_Y, z));
    }

    const int NUM_BEAVERS = 10;

    // --- 4. Beavers (Directly added to the scene) ---
    for (int i = 0; i < NUM_BEAVERS; i++)
    {
        DrawableObject* mole = new DrawableObject(beaverModel, shader);
        mole->setTexture(moleTexture);

        // Static Transforms
        mole->addStaticTransform(new Rotation(vec3(180.0f, 0.0f, 0.0f)));
        mole->addStaticTransform(new Scaling(vec3(0.5f, 0.5f, 0.5f)));

        // Dynamic Movement: Uses the pre-defined global path
        float speed = ROAMING_SPEED + static_cast<float>(rand()) / RAND_MAX * 0.00001f;

        // NOTE: All beavers will follow the SAME path, but at different offsets/speeds
        mole->addLocalTransform(new ParametricLineMovement(global_roaming_path, speed));

        scene->addEntity(mole);
    }

    return scene;
}*/

Scene* sceneWhacAMole()
{
    Scene* scene = new Scene();
    ResourceManager& rm = ResourceManager::getInstance();

    // 1. Resources
    ShaderProgram* shader = rm.loadShaderProgram("lambert.vert", "generalLight.frag");

    // Models & Textures
    Model* sphereModel = rm.loadModel("./Res/planet.obj");
    Texture* moleTexture = rm.loadTexture("./Res/beaver.jpg"); // Assuming a better texture
    Texture* boardTexture = rm.loadTexture("./Res/water.jpg");
 
    // ---
    
    // 3. Game Board (Parent Object)
    /*DrawableObject* board = new DrawableObject(cubeModel, shader);
    board->setTexture(boardTexture);
    
    // Place and size the board. We'll set its base at Z=0.25 (half of its scale).
	board->addStaticTransform(new Rotation(vec3(90, 0, 0)));
    board->addStaticTransform(new Scaling(vec3(6.0f, 6.0f, 0.5f))); 
    board->addStaticTransform(new Translation(vec3(0, 0, 0.25f))); 
    scene->addEntity(board);

    // 3x3 grid positions (Hole centers, relative to the board's center (0, 0))
    // Note: These positions are on the X-Y plane.
    vector<vec3> holePositions = {
        {-2.0f, -2.0f, 0.0f}, {0.0f, -2.0f, 0.0f}, {2.0f, -2.0f, 0.0f},
        {-2.0f, 0.0f, 0.0f},  {0.0f, 0.0f, 0.0f},  {2.0f, 0.0f, 0.0f},
        {-2.0f, 2.0f, 0.0f},  {0.0f, 2.0f, 0.0f},  {2.0f, 2.0f, 0.0f}
    };*/
    
    // 4. Moles (Child Objects)
    Model* beaverModel = rm.loadModel("./Res/beaver.obj");

    DrawableObject* beaver = new DrawableObject(beaverModel, shader);
	beaver->addStaticTransform(new Rotation(vec3(-90.0f, 0.0f, 0.0f))); // Orient the mole upright)
	beaver->addStaticTransform(new Scaling(vec3(0.1f, 0.1f, 0.1f))); // Initial scaling

    beaver->setTexture(moleTexture);
        

    vector<vec3> path = {
        vec3(0, 0.05f, 0), // Hidden (Static Z=0 is the board center, so Z=-0.5 is low)
        vec3(0, -3.0f, 0),   // Popped Up (0.5 units above the board center)
        vec3(0, 0.05f, 0)
    };

    beaver->addLocalTransform(new ParametricLineMovement(path, 0.1));
	scene->getWorldEntityGenerator()->setSampleEntity(beaver);

	scene->getWorldEntityGenerator()->setMinRange(vec3(-15.0f, 0, -15.0f));
	scene->getWorldEntityGenerator()->setMaxRange(vec3(15.0f, 0, 15.0f));

    beaver->setStencilIndex(1);

    //HERE IT works
	//scene->addEntity(beaver);

    //HERE IT WON'T WORK
	//scene->addEntity(beaver->getCopy());

    for (int i = 0; i < 10; i++)
    {
        //HERE IT WON'T WORK
        scene->addEntity(scene->getWorldEntityGenerator()->generateEntityRandomly());
    }

    DrawableObject* board = new DrawableObject(sphereModel, shader);
    board->setTexture(boardTexture);

    //board->addStaticTransform(new Rotation(vec3(90, 0, 0)));
    board->addStaticTransform(new Scaling(vec3(30.0f, 0.01f, 30.0f)));
    board->addStaticTransform(new Translation(vec3(0, 0, 1)));
    scene->addEntity(board);

    // 2. Static World Elements (Ground, Camera, Light)
// Ground
    DrawableObject* ground = new DrawableObject(rm.loadModel("./Res/teren.obj"), shader);
    ground->setTexture(rm.loadTexture("./Res/grass.png"));
    //ground->addStaticTransform(new Scaling(vec3(10.0f, 1.0f, 10.0f)));
    scene->addEntity(ground);

    // Light
    Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
    light->addStaticTransform(new Translation(vec3(50, 50, 50)));
    light->attach(shader);
    scene->addEntity(light);

	// Camera - MUST BE ALWAYS (when using Stencil buffer) LAST 
    Camera* camera = new Camera();
    camera->attach(shader);
    //camera->addStaticTransform(new Translation(vec3(0, -8, 5)));
    //camera->addStaticTransform(new Rotation(vec3(-30.0f, 0, 0)));
    scene->addEntity(camera);

    return scene;
}
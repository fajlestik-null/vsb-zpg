#include "Scene.h"

#include "ArraysOfVertices.h" //for compilation optimalization included in Scene.cpp
#include "ResourceManager.h"


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

    ShaderProgram* shaderProgram = new ShaderProgram(ShaderLoadType::FILE, "generalVertexShader.vert", "generalBlinnLight.frag");

    DrawableObject* entity = new DrawableObject(modelTriangle, shaderProgram, vec3(1, 0, 0));

    entity->getTransformManager()->addStaticTransform(new Scale(vec3(0.5f, 0.5f, 0.5f)));

    scene->addEntity(entity);

    Camera* camera = new Camera();
    camera->attach(shaderProgram);

    scene->addEntity(camera);

    Light* light = new Light(LightType::POINT, vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f);

    light->setModel(modelSphere);
    //light->addShaderProgram(shaderProgram);

    light->addStaticTransform(new Scale(vec3(0.2f, 0.2f, 0.2f)));
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
    ShaderProgram* shaderPhong = new ShaderProgram(ShaderLoadType::FILE, "generalVertexShader.vert", "generalPhongLight.frag");
    ShaderProgram* reflector = new ShaderProgram(ShaderLoadType::FILE, "generalVertexShader.vert", "reflector.frag");

    DrawableObject* drawableObject;
    vec3 objectColor = vec3(1.0f, 0.0f, 0.0f);

    model = new Model(plain);

    drawableObject = new DrawableObject(model, reflector, vec3(1, 0, 0));
    //

    drawableObject->addStaticTransform(new Translation(vec3(0, 0, -1)));
    drawableObject->addStaticTransform(new Rotation(vec3(0, 90, 90)));
    drawableObject->addStaticTransform(new Scale(vec3(2.5f, 1.0f, 2.5f)));

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
        drawableObject->addStaticTransform(new Scale(vec3(0.5f, 0.5f, 0.5f)));
        scene->addEntity(drawableObject);
    }


    Light* light = new Light(LightType::POINT, vec3(0.385, 0.647, 0.812), 1.0f, 1.0f);
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

    auto textureSkyBox = resourceManager.loadCubeMap("night", { "./Res/starfield1.jpg",
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

    Model* model = new Model();

    ShaderProgram* shader = new ShaderProgram(ShaderLoadType::FILE, "generalVertexShader.vert", "generalPhongLight.frag");
    ShaderProgram* shaderTexture = new ShaderProgram(ShaderLoadType::FILE, "generalVertexShader.vert", "generalPhongLight.frag");
    ShaderProgram* shaderGrass = resourceManager.loadShaderProgram("grass.vert", "generalPhongLight.frag");

    Texture* grass = resourceManager.loadTexture("./Res/grass.png");
    model = resourceManager.loadModel("./Res/teren.obj");


    DrawableObject* ground = new DrawableObject();

    ground = new DrawableObject(model, shaderGrass, vec3(0.5f, 0.2f, 0.2f));

    ground->setTexture(grass);

    ground->addStaticTransform(new Scale(vec3(0.06f, 0.06f, 0.06f)));
    ground->addStaticTransform(new Translation(vec3(2.5f, 0.0f, 3.6f)));
    ground->getMaterial()->setSpecularFactor(vec3(0));
    scene->addEntity(ground);

    DrawableObject* entity = new DrawableObject();

    model = new Model(tree);
    for (int i = 0; i < 50; i++)
    {

        entity = new DrawableObject(model, shader, vec3(0.42f, 0.126f, 0.25f));

        entity->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

        float random = (float)(rand() % 20) / (float)100 + 0.1f;
        entity->addStaticTransform(new Scale(vec3(random, random, random)));

        scene->addEntity(entity);
    }

    model = new Model(bushes);
    for (int i = 0; i < 50; i++)
    {

        entity = new DrawableObject(model, shader, vec3(0.124f, 0.252f, 0.0f));

        entity->addStaticTransform(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

        float random = (float)(rand() % 10) / (float)100 + 0.15f;
        entity->addStaticTransform(new Scale(vec3(random, random, random)));

        scene->addEntity(entity);
    }

    model = resourceManager.loadModel("./Res/shrek.obj");
    Texture* shrek_tx = resourceManager.loadTexture("./Res/shrek.png");


    entity = new DrawableObject(model, shaderTexture, vec3(0.0f, 0.1f, 0.0f));
    entity->setTexture(shrek_tx);

    entity->addStaticTransform(new Translation(vec3(2.0f, 0.0f, 1.9f)));
    entity->addStaticTransform(new Scale(vec3(0.1f, 0.1f, 0.1f)));

    scene->addEntity(entity);


    model = resourceManager.loadModel("./Res/fiona.obj");
    entity = new DrawableObject(model, shaderTexture, vec3(0.0f, 0.1f, 0.0f));

    Texture* fiona_tx = resourceManager.loadTexture("./Res/fiona.png");
    entity->setTexture(fiona_tx);

    entity->addStaticTransform(new Translation(vec3(2.0f, 0.0f, 2.1f)));
    entity->addStaticTransform(new Scale(vec3(0.1f, 0.1f, 0.1f)));

    scene->addEntity(entity);


    model = resourceManager.loadModel("./Res/toiled.obj");
    entity = new DrawableObject(model, shaderTexture, vec3(0.0f, 0.1f, 0.0f));

    Texture* toilet_tx = resourceManager.loadTexture("./Res/toiled.jpg");
    entity->setTexture(toilet_tx);

    entity->addStaticTransform(new Translation(vec3(2.0f, 0.0f, 2.0f)));
    entity->addStaticTransform(new Scale(vec3(0.1f, 0.1f, 0.1f)));

    scene->addEntity(entity);

    model = new Model(sphere);

    Light* light;


    for (int i = 0; i < 5; i++)
    {
        light = new Light(LightType::POINT, vec3(0.886f, 1.0f, 0.6f), 1.0f, 0.9f);
        light->setModel(model);
        light->addShaderProgram(shader);
        light->addStaticTransform(new Scale(vec3(0.002f, 0.002f, 0.002f)));
        light->addStaticTransform(new Translation(vec3(2.0f, 0.5f, 4.0f)));
        light->getMaterial()->setAmbientFactor(vec3(1.0f, 1.0f, 1.0f));
        light->getMaterial()->setDiffuseFactor(vec3(0.0f, 0.0f, 0.0f));
        light->getMaterial()->setSpecularFactor(vec3(0.0f, 0.0f, 0.0f));


        light->addLocalTransform(new TransformTimer({ {0.5, new Rotation(vec3(0.0f, -(40.0f), 0.0f), vec3(0.0f, (40.0f), 0.0f))}, {0, new Translation(vec3(0.005f, 0.0f, 0.0f))} })); //{min},{max}
        light->attach(shader);
        light->attach(shaderTexture);
        light->attach(shaderSky);
        light->attach(shaderGrass);
        scene->addEntity(light);
    }

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(shaderTexture);
    camera->attach(shaderSky);
    camera->attach(shaderGrass);
    scene->addEntity(camera);

    sky->addParent(camera->getTransformManager());
    sky->getTransformManager()->setInheritOnlyTransform(InheritTransformation::TRANSLATION);

    light = new Light(LightType::REFLECTOR, vec3(0.90f, 0.95f, 1.0f), 1.3f, 0.1f);
    //light->setModel(model);
    //light->addShaderProgram(shader);
    light->addParent(camera->getTransformManager());
    light->setActive(true);
    light->attach(shader);
    light->attach(shaderTexture);
    light->attach(shaderSky);
    light->attach(shaderGrass);
    scene->addEntity(light);

    light = new Light(LightType::AMBIENT, vec3(0.90f, 0.95f, 1.0f), 1.0f, 0.75f);
    //light->setModel(model);
    //light->addShaderProgram(shader);
    light->attach(shader);
    light->attach(shaderTexture);
    light->attach(shaderSky);
    light->attach(shaderGrass);
    scene->addEntity(light);

    return scene;
}

Scene* sceneSolarSystem()
{
    Scene* scene = new Scene();

    ResourceManager& resourceManager = ResourceManager::getInstance();

    auto textureSkyBox = resourceManager.loadCubeMap("night",
        { "./Res/starfield1.jpg",
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


    ShaderProgram* shader = resourceManager.loadShaderProgram("generalVertexShader.vert", "generalPhongLight.frag");
    Model* model = resourceManager.loadModel("./Res/planet.obj");

    float offset = -20.0f;
    float slowCoefficient = 10.0f;
    float earthGlobalRotation = 1.0f / slowCoefficient;
    float earthSelftRotation = 366.256f / slowCoefficient;

    Texture* texture = resourceManager.loadTexture("./Res/sun.jpg");
    DrawableObject* sun = new DrawableObject(model, shader);
    float sunSize = 15.0f;
    sun->addStaticTransform(new Translation(vec3(0.0f, offset, 0.0f)));
    sun->addStaticTransform(new Scale(vec3(sunSize, sunSize, sunSize)));
    sun->getMaterial()->setAmbientFactor(vec3(10.0f, 10.0f, 10.0f));
    sun->setTexture(texture);
    scene->addEntity(sun);

    texture = resourceManager.loadTexture("./Res/mercury.jpg");
    DrawableObject* mercury = new DrawableObject(model, shader);
    mercury->addStaticTransform(new Translation(vec3(sunSize + 17.96, offset / 2, 0.0f)));
    mercury->addStaticTransform(new Scale(vec3(0.38f, 0.38f, 0.38f)));
    mercury->addGlobalTransform(new Rotation(vec3(0.0f, 4.15f * earthGlobalRotation, 0.0f)));
    mercury->addLocalTransform(new Rotation(vec3(0.0f, 0.016f * earthSelftRotation, 0.0f)));
    mercury->setTexture(texture);
    mercury->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(mercury);

    texture = resourceManager.loadTexture("./Res/venus.jpg");
    DrawableObject* venus = new DrawableObject(model, shader);
    venus->addStaticTransform(new Translation(vec3(sunSize + 33.16f, offset / 2, 0.0f)));
    venus->addStaticTransform(new Scale(vec3(0.95f, 0.95f, 0.95f)));
    venus->addGlobalTransform(new Rotation(vec3(0.0f, 3.07f * earthGlobalRotation, 0.0f)));
    venus->addLocalTransform(new Rotation(vec3(0.0f, 0.004 * earthSelftRotation, 0.0f)));
    venus->setTexture(texture);
    venus->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(venus);

    texture = resourceManager.loadTexture("./Res/earth.jpg");
    DrawableObject* earth = new DrawableObject(model, shader);
    earth->addStaticTransform(new Translation(vec3(sunSize + 46.05f, offset / 2, 0.0f)));
    earth->addGlobalTransform(new Rotation(vec3(0.0f, earthGlobalRotation, 0.0f)));
    earth->addStaticTransform(new Scale(vec3(0.95f, 0.95f * 0.996f, 0.95f)));
    earth->addLocalTransform(new Rotation(vec3(0.0f, earthSelftRotation, 0.0f)));
    earth->setTexture(texture);
    earth->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(earth);

    texture = resourceManager.loadTexture("./Res/moon.jpg");
    DrawableObject* moon = new DrawableObject(model, shader);
    moon->addParent(earth->getTransformManager());
    moon->addLocalTransform(new Rotation(vec3(0.0f, (earthSelftRotation / 27.3f) / slowCoefficient, 0.0f)));
    moon->addStaticTransform(new Translation(vec3(2.8f, 0.0f, 0.0f)));
    moon->addStaticTransform(new Scale(vec3(0.27f)));
    moon->setTexture(texture);
    moon->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(moon);


    Model* loginModel = resourceManager.loadModel("./Res/login.obj");
    DrawableObject* login = new DrawableObject(loginModel, shader, vec3(0.8f, 0.7f, 0.6f));
    login->addParent(earth->getTransformManager());
    login->addLocalTransform(new Rotation(vec3(0.0f, 10 / slowCoefficient, 0.0f)));
    login->addStaticTransform(new Translation(vec3(-6.0f, 0.0f, 0.0f)));
    login->addStaticTransform(new Scale(vec3(0.5f)));
    login->getMaterial()->setSpecularFactor(vec3(0.0f));
    login->setTexture(resourceManager.loadTexture("./Res/grass.png"));
    scene->addEntity(login);


    DrawableObject* mars = new DrawableObject(model, shader);
    mars->addStaticTransform(new Translation(vec3(sunSize + 70.0f, offset / 2, 0.0f)));
    mars->addStaticTransform(new Scale(vec3(0.53f, 0.53f * 0.994f, 0.53f)));
    mars->addGlobalTransform(new Rotation(vec3(0.0f, 0.81f * earthGlobalRotation, 0.0f)));
    mars->addLocalTransform(new Rotation(vec3(0.0f, 0.97 * earthSelftRotation, 0.0f)));
    texture = resourceManager.loadTexture("./Res/mars.jpg");
    mars->setTexture(texture);
    mars->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(mars);


    Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 0.9f), 1.0f, 200.0f);
    light->addParent(sun->getTransformManager());
    light->attach(shaderSky);
    light->attach(shader);
    scene->addEntity(light);

    Light* lightA = new Light(LightType::AMBIENT, vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.0f);
    lightA->attach(shader);
    scene->addEntity(lightA);

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(shaderSky);
    scene->addEntity(camera);

    sky->addParent(camera->getTransformManager());
    sky->getTransformManager()->setInheritOnlyTransform(InheritTransformation::TRANSLATION);

    return scene;
}

Scene* sceneTesting()
{
    Scene* scene = new Scene();

    ResourceManager& resourceManager = ResourceManager::getInstance();

    auto textureSkyBox = resourceManager.loadCubeMap("night", { "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg",
                                         "./Res/starfield1.jpg" });


    Model* skybox = resourceManager.loadModel("./Res/skybox.obj");
    ShaderProgram* shaderSky = resourceManager.loadShaderProgram("skyBox.vert", "skyBox.frag");

    DrawableObject* sky = new DrawableObject(skybox, shaderSky, vec3(0.8f, 0.7f, 0.6f));


    sky->setTexture(textureSkyBox);
    scene->setSkyBox(sky);


    Model* model = resourceManager.loadModel("./Res/cube.obj");
    ShaderProgram* shader = resourceManager.loadShaderProgram("generalVertexShader.vert", "generalPhongLight.frag");
    Texture* wood = resourceManager.loadTexture("./Res/wooden_fence.png");

    DrawableObject* cube = new DrawableObject(model, shader, vec3(0.8f, 0.7f, 0.6f));

    cube->setTexture(wood);

    cube->addStaticTransform(new Custom(mat4(1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 20.0)));
    cube->setStencilIndex(10);

    scene->addEntity(cube);

    Model* cubeM = resourceManager.loadModel("./Res/cube.obj");

    DrawableObject* cube_en = new DrawableObject(cubeM, shader, vec3(0.8f, 0.7f, 0.6f));

    cube_en->addStaticTransform(new Translation(vec3(1.5f, 0.0f, 0.0f)));
    cube_en->setStencilIndex(20);

    scene->addEntity(cube_en);


    Model* shrek = resourceManager.loadModel("./Res/shrek.obj");
    Model* fiona = resourceManager.loadModel("./Res/fiona.obj");
    Texture* shrek_tx = resourceManager.loadTexture("./Res/shrek.png");


    DrawableObject* shrek_en = new DrawableObject(shrek, shader, vec3(0.0f, 0.1f, 0.0f));
    shrek_en->setTexture(shrek_tx);
    shrek_en->setStencilIndex(30);
    scene->addEntity(shrek_en);


    DrawableObject* fiona_en = new DrawableObject(fiona, shader, vec3(0.0f, 0.1f, 0.0f));

    Texture* fiona_tx = resourceManager.loadTexture("./Res/fiona.png");
    fiona_en->setTexture(fiona_tx);

    fiona_en->addStaticTransform(new Translation(vec3(-2.0f, 0.0f, 0.0f)));
    fiona_en->setStencilIndex(40);

    scene->addEntity(fiona_en);

    model = resourceManager.loadModel("./Res/beaver.obj");

    DrawableObject* beaver = new DrawableObject(model, shader, vec3(0.0f, 0.1f, 0.0f));

    Texture* beaver_tx = resourceManager.loadTexture("./Res/beaver.jpg");
    beaver->setTexture(beaver_tx);

    beaver->addStaticTransform(new Translation(vec3(5.0f, 0.0f, 3.0f)));
    beaver->addStaticTransform(new Scale(vec3(0.1f, 0.1f, 0.1f)));
    beaver->addStaticTransform(new Rotation(vec3(-90.0f, 0.0f, -90.0f)));
    beaver->addLocalTransform(new BezierCurveMovement({ vec3(5.0f, -2.0f, 3.0f),
                                                    vec3(5.0f, 2.0f, 3.0f),
                                                    vec3(7.0f, 2.0f, 3.0f),
                                                    vec3(7.0f, -2.0f, 3.0f) }, 1.0f));

    beaver->setStencilIndex(50);

    scene->addEntity(beaver);

    ShaderProgram* shaderGrass = resourceManager.loadShaderProgram("grass.vert", "generalPhongLight.frag");

    DrawableObject* terain = new DrawableObject(resourceManager.loadModel("./Res/teren.obj"), shaderGrass, vec3(0.0f, 0.1f, 0.0f));

    terain->setTexture(resourceManager.loadTexture("./Res/grass.png"));

    terain->getMaterial()->setSpecularFactor(vec3(0.0f));

    scene->addEntity(terain);

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

Scene* sceneWhacAMole()
{
    Scene* scene = new Scene();
    ResourceManager& rm = ResourceManager::getInstance();

    ShaderProgram* shader = rm.loadShaderProgram("generalVertexShader.vert", "generalPhongLight.frag");
    ShaderProgram* shaderGrass = rm.loadShaderProgram("grass.vert", "generalPhongLight.frag");

    Model* sphereModel = rm.loadModel("./Res/planet.obj");
    Texture* moleTexture = rm.loadTexture("./Res/beaver.jpg");
    Texture* boardTexture = rm.loadTexture("./Res/water.jpg");
    Model* beaverModel = rm.loadModel("./Res/beaver.obj");
    Model* skybox = rm.loadModel("./Res/skybox.obj");
    Model* modelTree = new Model(tree);

    ShaderProgram* shaderSky = rm.loadShaderProgram("skyBox.vert", "skyBox.frag");

    auto textureSkyBox = rm.loadCubeMap("day", { "./Res/posx.jpg",
                                                     "./Res/negx.jpg",
                                                     "./Res/posy.jpg",
                                                     "./Res/negy.jpg",
                                                     "./Res/posz.jpg",
                                                     "./Res/negz.jpg" });


    DrawableObject* sky = new DrawableObject(skybox, shaderSky, vec3(0.8f, 0.7f, 0.6f));
    sky->setTexture(textureSkyBox);
    scene->setSkyBox(sky);

    DrawableObject* beaver = new DrawableObject(beaverModel, shader);
    beaver->addStaticTransform(new Rotation(vec3(-90.0f, 0.0f, -90.0f)));
    beaver->addStaticTransform(new Scale(vec3(0.1f, 0.1f, 0.1f)));
    beaver->getMaterial()->setSpecularFactor(vec3(0.75));


    beaver->setTexture(moleTexture);

    scene->getWorldEntityGenerator()->setSampleEntity(beaver);

    scene->getWorldEntityGenerator()->setMinRange(vec3(-15.0f, 0, -15.0f));
    scene->getWorldEntityGenerator()->setMaxRange(vec3(15.0f, 0, 15.0f));

    beaver->setStencilIndex(1);

    vector<vec3> path = { vec3(5.0f, -2.0f, 3.0f),
                         vec3(5.0f, 2.0f, 3.0f),
                         vec3(7.0f, 2.0f, 3.0f),
                         vec3(7.0f, -2.0f, 3.0f) }
    ;

    for (int i = 0; i < 9; i++)
    {
        auto newBeaver = scene->getWorldEntityGenerator()->generateEntityRandomly();
        newBeaver->addLocalTransform(new BezierCurveMovement(path, 0.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.5f)))));
        scene->addEntity(newBeaver);
    }

    beaver->addLocalTransform(new BezierCurveMovement(path, 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.5f)))));

    DrawableObject* treeEn = new DrawableObject();

    vec3 pondCenter = vec3(0, 0, 1);
    float pondRadius = 30.0f;

    for (int i = 0; i < 20; i++)
    {
        treeEn = new DrawableObject(modelTree, shader, vec3(0.65f, 0.16f, 0.16f));

        vec3 pos;
        do {
            pos.x = static_cast<float>(rand() % 50) - 25.0f;
            pos.z = static_cast<float>(rand() % 50) - 25.0f;
            pos.y = 0.0f;
        } while (length(vec2(pos.x - pondCenter.x, pos.z - pondCenter.z)) < pondRadius);

        treeEn->addStaticTransform(new Translation(pos));

        float random = static_cast<float>(rand() % 20) / 100.0f + 0.5f;
        treeEn->addStaticTransform(new Scale(vec3(random, random, random)));

        treeEn->getMaterial()->setSpecularFactor(vec3(0.0f));

        scene->addEntity(treeEn);
    }

    DrawableObject* pond = new DrawableObject(sphereModel, shader);
    pond->setTexture(boardTexture);

    pond->addStaticTransform(new Scale(vec3(pondRadius, 0.01f, pondRadius)));
    pond->addStaticTransform(new Translation(pondCenter));
    pond->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(pond);


    DrawableObject* ground = new DrawableObject(rm.loadModel("./Res/teren.obj"), shaderGrass);
    ground->setTexture(rm.loadTexture("./Res/grass.png"));
    ground->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(ground);

    Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
    light->addStaticTransform(new Translation(vec3(50, 50, 50)));
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

Scene* sceneLogin()
{
    Scene* scene = new Scene();
    ResourceManager& rm = ResourceManager::getInstance();

    ShaderProgram* shader = rm.loadShaderProgram("generalVertexShader.vert", "generalPhongLight.frag");

    Model* model = rm.loadModel("./Res/login.obj");
    DrawableObject* login = new DrawableObject(model, shader, vec3(0.8f, 0.7f, 0.6f));
    login->setTexture(rm.loadTexture("./Res/grass.png"));
    scene->addEntity(login);


    Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
    light->addStaticTransform(new Translation(vec3(50, 50, 50)));
    light->attach(shader);
    scene->addEntity(light);

    Camera* camera = new Camera();
    camera->attach(shader);
    scene->addEntity(camera);

    return scene;
}

Scene* sceneFormula()
{
    Scene* scene = new Scene();
    ResourceManager& rm = ResourceManager::getInstance();

    Model* skyModel = rm.loadModel("./Res/skybox.obj");
    Model* terenModel = rm.loadModel("./Res/teren.obj");
    Model* formulaModel = rm.loadModel("./Res/formula1.obj");

    ShaderProgram* shader = rm.loadShaderProgram("generalVertexShader.vert", "generalPhongLight.frag");
    ShaderProgram* skyShader = rm.loadShaderProgram("skyBox.vert", "skyBox.frag");
    ShaderProgram* grassShader = rm.loadShaderProgram("grass.vert", "generalPhongLight.frag");

    Texture* grassTextre = rm.loadTexture("./Res/grass.png");


    DrawableObject* sky = new DrawableObject(skyModel, skyShader);

    auto skyTexture = rm.loadCubeMap("day", { "./Res/posx.jpg",
                                                     "./Res/negx.jpg",
                                                     "./Res/posy.jpg",
                                                     "./Res/negy.jpg",
                                                     "./Res/posz.jpg",
                                                     "./Res/negz.jpg" });

    sky->setTexture(skyTexture);
    scene->setSkyBox(sky);

    DrawableObject* formula = new DrawableObject(formulaModel, shader, vec3(1.0f, 0.0f, 0.0f));
    formula->addStaticTransform(new Scale(vec3(0.05f, 0.05f, 0.05f)));
    formula->addStaticTransform(new Rotation(vec3(0.0f, 90.f, 0.0f)));
    formula->addLocalTransform(new BezierCurveMovement({
    vec3(0.0f, 0.0f, 0.0f),
    vec3(7.0f, 0.0f, 9.0f),
    vec3(10.0f, 0.0f, 6.0f),
    vec3(15.0f, 0.0f, 3.0f),
    vec3(20.0f, 0.0f, 0.0f),
    vec3(25.0f, 0.0f, -5.0f),
    vec3(20.0f, 0.0f, -10.0f) }, 0.1f
    ));
    formula->getMaterial()->setSpecularFactor(vec3(0.5f, 0.5f, 0.5f));
    formula->getMaterial()->setShininess(40);
    scene->addEntity(formula);


    DrawableObject* ground = new DrawableObject(terenModel, grassShader);
    ground->setTexture(grassTextre);
    ground->getMaterial()->setSpecularFactor(vec3(0.0f));
    scene->addEntity(ground);

    Light* light = new Light(LightType::POINT, vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
    light->addStaticTransform(new Translation(vec3(50, 50, 50)));
    light->attach(shader);
    light->attach(skyShader);
    light->attach(grassShader);
    scene->addEntity(light);

    Camera* camera = new Camera();
    camera->attach(shader);
    camera->attach(skyShader);
    camera->attach(grassShader);
    scene->addEntity(camera);

    sky->addParent(camera->getTransformManager());
    sky->getTransformManager()->setInheritOnlyTransform(InheritTransformation::TRANSLATION);

    return scene;
}
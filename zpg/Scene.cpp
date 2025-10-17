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
    std::vector<float>triangle = {
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    Scene* s = new Scene();
    IObserver* observer = new ShaderProgram(ShaderLoadType::FILE, "constant.vert", "constant.frag");
    s->addCameraObserver(observer);
    DrawableObject* object = new DrawableObject(new Model(triangle), (ShaderProgram*)observer, new Rotation(vec3(0.0f, 0.0f, 0.0f)));
    s->addObject(object);
    return s;
}

Scene* sceneSpheres()
{
    Scene* scene = new Scene();

    Model* modelSphere;
    IObserver* observer = new ShaderProgram(ShaderLoadType::FILE, "lambert.vert", "phong.frag");
    scene->setLight(new Light(vec3(0.0), vec3(0.385, 0.647, 0.812)), observer);
	scene->addCameraObserver(observer);
    DrawableObject* drawableObject;


    std::vector<DrawableObject*> setterCollection;

    for (int i = 0; i < 4; i++)
    {
        modelSphere = new Model(sphere);


        switch (i)
        {
        default:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram *) observer, new Translation(vec3(0.0f, 1.0f, 0.0f)));
            break;
        case 1:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram*)observer, new Translation(vec3(1.0f, 0.0f, 0.0f)));
            break;
        case 2:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram*)observer, new Translation(vec3(-1.0f, 0.0f, 0.0f)));
            break;
        case 3:
            drawableObject = new DrawableObject(modelSphere, (ShaderProgram*)observer, new Translation(vec3(0.0f, -1.0f, 0.0f)));
            break;
        }
		drawableObject->staticTransformation(new Scaling(vec3(0.5f, 0.5f, 0.5f)));
        scene->addObject(drawableObject);
    }
	return scene;
}

/*
Scene* sceneMess()
{
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vertexColor;"
        "out vec3 fragmentColor;"
        "uniform mat4 modelMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 projectionMatrix;"
        "void main () {"
        "     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);"
        "     fragmentColor = vertexColor;"
        "}";

    const char* fragment_shader =
        "#version 330\n"
        "in vec3 fragmentColor;"
        "out vec3 fragColor;"
        "void main () {"
        "     fragColor = fragmentColor;"
        "}";

    const char* vertex_shader_n =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "uniform mat4 modelMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 projectionMatrix;"
        "void main () {"
        "     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);"
        "}";

    const char* fragment_shader_1 =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4 (0.5, 0.0, 0.5, 1.0);"
        "}";

    const char* fragment_shader_2 =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4 (1.0, 0.0, 0.0, 1.0);"
        "}";

    const char* fragment_shader_3 =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4 (0.0, 1.0, 0.0, 1.0);"
        "}";
    
    const char* fragment_shader_4 =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4 (0.0, 0.0, 1.0, 1.0);"
        "}";

    Scene* scene = new Scene();

    Model* model;
    IObserver* observer0 = new ShaderProgram(ShaderLoadType::VARIABLE,vertex_shader, fragment_shader);
    IObserver* observer1 = new ShaderProgram(ShaderLoadType::VARIABLE, vertex_shader_n, fragment_shader_1);
    IObserver* observer2 = new ShaderProgram(ShaderLoadType::VARIABLE, vertex_shader_n, fragment_shader_2);
    IObserver* observer3 = new ShaderProgram(ShaderLoadType::VARIABLE, vertex_shader_n, fragment_shader_3);
    IObserver* observer4 = new ShaderProgram(ShaderLoadType::VARIABLE, vertex_shader_n, fragment_shader_4);
	scene->addCameraObserver(observer0);
	scene->addCameraObserver(observer1);
	scene->addCameraObserver(observer2);
    scene->addCameraObserver(observer3);
	scene->addCameraObserver(observer4);

    DrawableObject* drawableObject;

	std::vector<DrawableObject*> objectsCollection;
    std::vector<std::vector<float>> verticesCollection = { bushes,sphere,suziSmooth};

    for (int i = 0; i < 30; i++)
    {
        int modelIndex = rand() % verticesCollection.size();
        model = new Model(verticesCollection[modelIndex]);

        switch (rand() % 5)
        {
        case 0:
            drawableObject = new DrawableObject(model, (ShaderProgram*) observer0);
            break;
        case 1:
            drawableObject = new DrawableObject(model, (ShaderProgram*)observer1);
            break;
		case 2:
            drawableObject = new DrawableObject(model, (ShaderProgram*)observer2);
			break;
        case 3:
            drawableObject = new DrawableObject(model, (ShaderProgram*)observer3);
			break;
		case 4:
            drawableObject = new DrawableObject(model, (ShaderProgram*)observer4);
			break;
        default:
            drawableObject = new DrawableObject(model, (ShaderProgram*) observer0);
			break;
        }

        drawableObject->staticTransformation(new Translation(vec3((float)(8 - rand() % 16) / 10, (float)(8 - rand() % 16) / 10, (float)(8 - rand() % 16) / 10)));

        float modelScale = (float)(rand() % 10) / (float)100 + 0.1f;
        drawableObject->staticTransformation(new Scaling(vec3(modelScale, modelScale, modelScale)));
        objectsCollection.push_back(drawableObject);
    }

    for (auto& oc : objectsCollection)
    {
        scene->addObject(oc);
    }

    return scene;
    
}*/

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

	//ask Marko why x and z are swapped
	obj->staticTransformation(new Scaling(vec3(2.5f, 1.0f, 2.5f))); //x,z,y
	obj->staticTransformation(new Translation(vec3(1.0f, 0.0f, 1.0f))); //x,z,y

	scene->addObject(obj);

    for (int i = 0; i < 100; i++)
    {

        model = new Model(tree);

        obj = new DrawableObject(model,(ShaderProgram*) observer);

        obj->staticTransformation(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

        float random = (float)(rand() % 20) / (float)100 + 0.1f;
        obj->staticTransformation(new Scaling(vec3(random, random, random)));

        scene->addObject(obj);
    }

    for (int i = 0; i < 100; i++)
    {
        model = new Model(bushes);

        obj = new DrawableObject(model, (ShaderProgram *) observer);

        obj->staticTransformation(new Translation(vec3(rand() % 50 / (float)10, 0.0f, rand() % 50 / (float)10)));

        float random = (float)(rand() % 10) / (float)100 + 0.15f;
        obj->staticTransformation(new Scaling(vec3(random, random, random)));

        scene->addObject(obj);
    }

	return scene;
}



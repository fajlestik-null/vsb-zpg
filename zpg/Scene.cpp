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

   // mCamera->recalculateCameraVectors();


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
        ICameraObserver* cameraObserver = new ShaderProgram(vertex_shader, fragment_shader);
        Scene* s = new Scene();
        DrawableObject* object = new DrawableObject(new Model(tree), (ShaderProgram *) cameraObserver, new Rotation(glm::vec3(10.0f, 0.0f, 0.0f)));
        s->addObject(object);
		s->addCameraObserver(cameraObserver);
        return s;
    
}

Scene* sceneTriangle()
{
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "uniform mat4 modelMatrix;"
        "void main () {"
        "     gl_Position = modelMatrix * vec4 (vp, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4 (0.5, 0.0, 0.5, 1.0);"
        "}";

    std::vector<float>triangle = {
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    Scene* s = new Scene();
    DrawableObject* object = new DrawableObject(new Model(triangle), new ShaderProgram(vertex_shader, fragment_shader), new Rotation(glm::vec3(10.0f, 0.0f, 0.0f)));
    object->addTransformation(new Rotation(glm::vec3(0.0f, 100.0f, 0.0f)));
    s->addObject(object);
    return s;
}


Scene* sceneSpheres()
{
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vertexColor;"
        "out vec3 fragmentColor;"
        "uniform mat4 modelMatrix;"
        "void main () {"
        "     gl_Position = modelMatrix * vec4 (vp, 1.0);"
        "     fragmentColor = vertexColor;"
        "}";

    const char* fragment_shader =
        "#version 330\n"
        "in vec3 fragmentColor;"
        "out vec3 fragColor;"
        "void main () {"
        "     fragColor = fragmentColor;"
        "}";

    Scene* scene = new Scene();

    Model* modelSphere;
    ShaderProgram* shaderProgram;
    DrawableObject* drawableObject;

    std::vector<DrawableObject*> setterCollection;

    for (int i = 0; i < 4; i++)
    {
        modelSphere = new Model(sphere);

        shaderProgram = new ShaderProgram(vertex_shader, fragment_shader);

        switch (i)
        {
        default:
            drawableObject = new DrawableObject(modelSphere, shaderProgram, new Scaling(glm::vec3(0.15f, 0.15f, 0.15f)));
            drawableObject->staticTransformation(new Translation(glm::vec3(0.0f, 5.0f, 0.0f)));
            break;
        case 1:
            drawableObject = new DrawableObject(modelSphere, shaderProgram, new Scaling(glm::vec3(0.15f, 0.15f, 0.15f)));
            drawableObject->staticTransformation(new Translation(glm::vec3(5.0f, 0.0f, 0.0f)));
            break;
        case 2:
            drawableObject = new DrawableObject(modelSphere, shaderProgram, new Scaling(glm::vec3(0.15f, 0.15f, 0.15f)));
            drawableObject->staticTransformation(new Translation(glm::vec3(-5.0f, 0.0f, 0.0f)));
            break;
        case 3:
            drawableObject = new DrawableObject(modelSphere, shaderProgram, new Scaling(glm::vec3(0.15f, 0.15f, 0.15f)));
            drawableObject->staticTransformation(new Translation(glm::vec3(0.0f, -5.0f, 0.0f)));
            break;
        }
        scene->addObject(drawableObject);
    }
	return scene;
}

Scene* sceneMess()
{
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vertexColor;"
        "out vec3 fragmentColor;"
        "uniform mat4 modelMatrix;"
        "void main () {"
        "     gl_Position = modelMatrix * vec4 (vp, 1.0);"
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
        "void main () {"
        "     gl_Position = modelMatrix * vec4 (vp, 1.0);"
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
    ShaderProgram* shaderProgram;
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
            shaderProgram = new ShaderProgram(vertex_shader, fragment_shader);
            break;
        case 1:
            shaderProgram = new ShaderProgram(vertex_shader_n, fragment_shader_1);
            break;
		case 2:
			shaderProgram = new ShaderProgram(vertex_shader_n, fragment_shader_2);
			break;
        case 3:
			shaderProgram = new ShaderProgram(vertex_shader_n, fragment_shader_3);
			break;
		case 4:
			shaderProgram = new ShaderProgram(vertex_shader_n, fragment_shader_4);
			break;
        default:
            shaderProgram = new ShaderProgram(vertex_shader_n, fragment_shader_1);
			break;
        }

        drawableObject = new DrawableObject(model, shaderProgram);

        drawableObject->staticTransformation(new Translation(glm::vec3((float)(8 - rand() % 16) / 10, (float)(8 - rand() % 16) / 10, (float)(8 - rand() % 16) / 10)));

        float modelScale = (float)(rand() % 10) / (float)100 + 0.1f;
        drawableObject->staticTransformation(new Scaling(glm::vec3(modelScale, modelScale, modelScale)));
        objectsCollection.push_back(drawableObject);
    }

    for (auto& oc : objectsCollection)
    {
        scene->addObject(oc);
    }

    return scene;
    
}


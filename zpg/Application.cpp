#include "Application.h"

Application::Application(const int WINDOW_WIDTH, const int WINDOW_HEIGHT)
	: mWindowWidth(WINDOW_WIDTH), mWindowHeight(WINDOW_HEIGHT), mWindow(NULL)
{
	mGeneralScene = new Scene();
}

Application::~Application() { terminate(); }

bool Application::init() {
    glfwSetErrorCallback(Controls::errorCallback);

    if (!glfwInit()) {
        cerr << "ERROR: could not start GLFW3\n";
        return false;
    }

    // Specify OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "ZPG", NULL, NULL);
    if (!mWindow) {
        cerr << "ERROR: could not create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "ERROR: could not initialize GLEW\n";
        return false;
    }

    // Print OpenGL info
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    cout << "Using GLEW " << glewGetString(GLEW_VERSION) << "\n";
    cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    cout << "Using GLFW " << major << "." << minor << "." << revision << "\n";

    // Set initial viewport size
    glfwGetFramebufferSize(mWindow, &mWindowWidth, &mWindowHeight);
    glViewport(0, 0, mWindowWidth, mWindowHeight);

	// Attach Control to window
	mControls->attachToWindow(mWindow);

    mGeneralScene = new Scene();

	//enable z-buffering
    glEnable(GL_DEPTH_TEST);

	// Enable stencil testing
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    return true;
}

void Application::run() {

    float lastTick = (float)glfwGetTime();
    float currentTick = 0;
    float deltaTime = 0;


    while (!glfwWindowShouldClose(mWindow)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // Clear frame  | GL_STENCIL_BUFFER_BIT

        switchScene({ sceneDefault, sceneSpheres, sceneTreesAndBushes, sceneSolarSystem, sceneWhacAMole, sceneTesting, sceneLogin, sceneFormula, sceneTestNormalMapping });
        currentTick = (float)glfwGetTime();
        deltaTime = currentTick - lastTick;
        lastTick = currentTick;
        mGeneralScene->render(mWindow,deltaTime, mControls);

        mControls->nextFrame();
        glfwPollEvents();         // Poll for events
        glfwSwapBuffers(mWindow);  // Swap buffers
    }
}

void Application::switchScene(vector<Scene* (*)()> scenes)
{
    for (int i = 0; i < (int)scenes.size(); i++)
    {
        if (glfwGetKey(mWindow, GLFW_KEY_1 + i) == GLFW_PRESS)
        {
            delete mGeneralScene;
            mGeneralScene = scenes[i]();
        }
    }
}

GLFWwindow* Application::GetWindow() const
{
    return mWindow;
}
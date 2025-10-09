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
        std::cerr << "ERROR: could not start GLFW3\n";
        return false;
    }

    // Specify OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "ZPG", NULL, NULL);
    if (!mWindow) {
        std::cerr << "ERROR: could not create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: could not initialize GLEW\n";
        return false;
    }

    // Print OpenGL info
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << "\n";
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "Using GLFW " << major << "." << minor << "." << revision << "\n";

    // Set initial viewport size
    glfwGetFramebufferSize(mWindow, &mWindowWidth, &mWindowHeight);
    glViewport(0, 0, mWindowWidth, mWindowHeight);

	// Attach controls to window
	mControls->attachToWindow(mWindow);

    mGeneralScene = new Scene();

    return true;
}

void Application::run() {

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(mWindow)) {

		mGeneralScene->processCamera(mWindow, mWindowWidth, mWindowHeight, mControls);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear frame

        
        switchScene({ sceneDefault, sceneTriangle, sceneSpheres, sceneMess});
        
        mGeneralScene->render();

        glfwSwapBuffers(mWindow);  // Swap buffers
        glfwPollEvents();
    }
}

void Application::switchScene(std::vector<Scene* (*)()> scenes)
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
#include "Application.h"

Application::Application(const int WINDOW_WIDTH, const int WINDOW_HEIGHT)
	: mWindowWidth(WINDOW_WIDTH), mWindowHeight(WINDOW_HEIGHT), mWindow(NULL) {}

Application::~Application() { terminate(); }

bool Application::init() {
    glfwSetErrorCallback(errorCallback);

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

    // Register callbacks
    //K èemu? - DOTAZ
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetWindowFocusCallback(mWindow, windowFocusCallback);
    glfwSetWindowIconifyCallback(mWindow, windowIconifyCallback);
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    glfwSetCursorPosCallback(mWindow, cursorCallback);
    glfwSetMouseButtonCallback(mWindow, buttonCallback);

    return true;
}

void Application::run() {

    glm::mat4 M = glm::mat4(1.0f); // construct identity matrix

    //M = glm::rotate(glm::mat4(1.0f), glm::radians(2.5f), glm::vec3(0.0f, 1.0f, 0.0f));
    //M = glm::rotate(M, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // M = glm::translate(M, glm::vec3(0.0f, 1.0f, 1.0));
    M = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)); //proè nelze využít pùvodní matici M? -DOTAZ
    //M = glm::rotate(M, glm::radians(rads), glm::vec3(0.0f, 0.0f, 1.0f));
    //M = glm::translate(M, glm::vec3(0.0f, 0.001f, 0.0f));
    float rads = 0.5;
    while (!glfwWindowShouldClose(mWindow)) {


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear frame
        
        mShaderPrograms[2]->use(M);
        mModels[1]->put();
        
        mShaderPrograms[1]->use(M);
        mModels[0]->put();

        glfwSwapBuffers(mWindow);  // Swap buffers
        glfwPollEvents();
    }
}

bool Application::terminate() {
    if (mWindow) {
        glfwDestroyWindow(mWindow);
        mWindow = NULL;
    }
    glfwTerminate();
    return true;
}

void Application::createShaders()
{
    const char* vertex_shader =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "void main () {"
        "     gl_Position = vec4 (vp, 1.0);"
        "}";
    const char* vertex_shader_trns =
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

    const char* fragment_shader_t =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4 (0.5, 0.25, 0.75, 1.0);"
        "}";


    const char* vertex_shader_n =
        "#version 330 core\n"
        "layout(location = 0) in vec3 pos;"
        "layout(location = 1) in vec3 color;"
        "out vec3 vertexColor;"
        "void main() {"
        "    gl_Position = vec4(pos, 1.0);"
        "    vertexColor = color;"
        "}";

    const char* fragment_shader_n =
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 fragColor;"
        "void main() {"
        "    fragColor = vec4(vertexColor, 1.0);"
        "}";

    ShaderProgram *sp = new ShaderProgram(vertex_shader, fragment_shader);
    mShaderPrograms.push_back(sp);
    sp = new ShaderProgram(vertex_shader_trns, fragment_shader_t);
    mShaderPrograms.push_back(sp);
    sp = new ShaderProgram(vertex_shader_n, fragment_shader_n);
    mShaderPrograms.push_back(sp);
}

void Application::createModels()
{
    std::vector<float> square = {
        -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.25f, 0.25f, 0.0f, 1.0f, 1.0f, 0.0f,
        - 0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    std::vector<float>triangle = {
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    Model* m = new Model(square);
    mModels.push_back(m);
    m = new Model(triangle);
    mModels.push_back(m);
}

void Application::errorCallback(int error, const char* description) { fputs(description, stderr); }

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

void Application::windowFocusCallback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }

void Application::windowIconifyCallback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

void Application::windowSizeCallback(GLFWwindow* window, int width, int height) {
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

void Application::cursorCallback(GLFWwindow* window, double x, double y) { printf("cursor_callback \n"); }

void Application::buttonCallback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
}
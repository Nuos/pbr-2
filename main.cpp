
#include <iostream>

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

using namespace gl;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

#include <memory>

#include "Timer.h"
#include "Scene.h"
#include "Sphere.h"
#include "PhongMaterial.h"
#include "Earth.h"
#include "Moon.h"
#include "Axis.h"
#include "Animator.h"

static int  s_WindowWidth  = 800;
static int  s_WindowHeight = 600;
static bool s_bEnableVSync = true;
static bool s_bWireframe   = false;
static bool s_bEarthScene = true;
static bool s_bFullScreen = false;

static std::shared_ptr<Scene> scene = std::make_shared<Scene>();

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->draw();
}

void key(GLFWwindow* /*window*/, int key, int /*s*/, int action, int /*mods*/)
{
	scene->camera()->keyEvent(key, action);

	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_L:
			//scene->toggleLightAnimation();
			break;
		case GLFW_KEY_K:
			//scene->toggleLightDirection();
			break;
		case GLFW_KEY_O:
			s_bWireframe = !s_bWireframe;
			glPolygonMode(GL_FRONT_AND_BACK, s_bWireframe ? GL_LINE : GL_FILL);			
			std::cout << "Wireframe: " << s_bWireframe << std::endl;
			break;		
		default:
			break;
		}		
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
    {
		return;
    }

	if (action == GLFW_PRESS)
	{
        double cursorX, cursorY;
        
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window, &cursorX, &cursorY);
        
        scene->camera()->mouseButtonEvent(cursorX, cursorY);
	}
	else
    {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
        scene->camera()->mouseMotionEvent(x, y);
    }
}

void mouseScrollCallback(GLFWwindow* /*window*/, double x, double y)
{
    scene->camera()->mouseScrollEvent(x, y);
}

void reshape(GLFWwindow* /*window*/, int width, int height)
{
	if (width > 0 && height > 0)
	{
		scene->camera()->setViewportSize(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
	}    
}

static void createScene()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	auto phongMaterial = std::make_shared<PhongMaterial>();
	auto sphereMesh = std::make_shared<SphereMesh>(200);

	auto earth = std::make_shared<Earth>(glm::vec3{ 0.0f }, 6.371f, sphereMesh);
	auto moon = std::make_shared<Moon>(glm::vec3{ 384.400f, 0.0f, 0.0f }, 10.737f, sphereMesh);

	auto axis = std::make_shared<Axis>();

	const float MoonRotationSpeed = 0.3f;

	auto moonAnimator = std::make_shared<Animator>(moon->transform);
	moonAnimator->RotationSpeed.z = MoonRotationSpeed;
	moonAnimator->WorldRotationSpeed.y = -MoonRotationSpeed;

	auto sunAnimator = std::make_shared<Animator>(scene->light->transform);
	sunAnimator->WorldRotationSpeed.y = -0.03f;

	scene->addAnimator(moonAnimator);
	scene->addAnimator(sunAnimator);

	// Order is important here. Earth must be the last
	scene->addDrawable(moon);
	scene->addDrawable(earth);
	scene->addDrawable(axis);
}

//static void createDefaultScene()
//{
//	glClearColor(0.1, 0.1, 0.1, 1.0);
//
//    auto mesh = new SphereMesh(200);
//    
//    auto simple = new SimpleMaterial;
//    //auto simpleTex = new SimpleTextureMaterial("textures/earth_8k.jpg");
//    auto phong = new PhongMaterial;
//    
//    scene->addDrawable(new Sphere({ -1.5, 0.0, 0.0 }, 0.5, mesh, phong));
//    scene->addDrawable(new Sphere({ -0.5, 0.0, 0.0 }, 0.5, mesh, simple));
//    scene->addDrawable(new Sphere({ 0.5, 0.0, 0.0 }, 0.5, mesh, phong));
//    scene->addDrawable(new Sphere({ 1.5, 0.0, 0.0 }, 0.5, mesh, simple));
//    
//    auto earthNoShadingSphere = new Sphere({ -1.5, 1.0, 0.0 }, 0.5, mesh, simple);
//	earthNoShadingSphere->transform().rotation = glm::vec3{ glm::radians(90.0f), 0.0f, glm::radians(10.0f) };
//    auto earthAnimator = new SphereAnimator(earthNoShadingSphere);
//    earthAnimator->setRotationSpeed({0.0f, 0.0f, glm::radians(10.0f)});
//    scene->addAnimator(earthAnimator);
//    
//    scene->addDrawable(earthNoShadingSphere);
//    scene->addDrawable(new Sphere({ -0.5, 1.0, 0.0 }, 0.5, mesh, simple));
//    scene->addDrawable(new Sphere({ 0.5, 1.0, 0.0 }, 0.5, mesh, simple));
//    scene->addDrawable(new Sphere({ 1.5, 1.0, 0.0 }, 0.5, mesh, simple));
//    
//    scene->addDrawable(new Sphere({ -1.5, -1.0, 0.0 }, 0.5, mesh, simple));
//    scene->addDrawable(new Sphere({ -0.5, -1.0, 0.0 }, 0.5, mesh, simple));
//    scene->addDrawable(new Sphere({ 0.5, -1.0, 0.0 }, 0.5, mesh, simple));
//    scene->addDrawable(new Sphere({ 1.5, -1.0, 0.0 }, 0.5, mesh, simple));    
//}

//static void createEarthScene1()
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);    
//
//	auto mesh = new SphereMesh(200);
//	auto material = new EarthMaterial;
//
//	auto earthSphere = new Sphere{ {0.0f, 0.0f, 0.0f}, 1.0f, mesh, material };
//	earthSphere->transform().rotation = glm::vec3{ glm::radians(90.0f), 0.0f, glm::radians(0.0f) };
//
//	auto earthAnimator = new SphereAnimator{ earthSphere };
//	earthAnimator->setRotationSpeed({ 0.0f, 0.0f, glm::radians(0.0f) });
//
//	scene->addDrawable(earthSphere);
//	scene->addAnimator(earthAnimator);
//}

static void init()
{
    // OpenGL Initialization
    using namespace std;
	using namespace glbinding;

    cout << "OpenGL Version: " << endl;
    cout << "\tGL_RENDERER: " << glbinding::ContextInfo::renderer() << endl;
    cout << "\tGL_VERSION : " << glbinding::ContextInfo::version() << endl;
    cout << "\tGL_VENDOR  : " << glbinding::ContextInfo::vendor() << endl;

	setCallbackMaskExcept(CallbackMask::After, { "glGetError" });
	setAfterCallback([](const FunctionCall &)
	{
		const auto error = glGetError();
		if (error != GL_NO_ERROR)
			std::cout << "error: " << std::hex << error << std::endl;
	});
	/*setCallbackMask(CallbackMask::After | CallbackMask::ParametersAndReturnValue);
	glbinding::setAfterCallback([](const glbinding::FunctionCall & call)
	{
		std::cout << call.function->name() << "(";
		for (unsigned i = 0; i < call.parameters.size(); ++i)
		{
			std::cout << call.parameters[i]->asString();
			if (i < call.parameters.size() - 1)
				std::cout << ", ";
		}
		std::cout << ")";

		if (call.returnValue)
			std::cout << " -> " << call.returnValue->asString();

		std::cout << std::endl;
	});*/
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);   
	
	createScene();
}

int main()
{
    GLFWwindow* window = nullptr;
    
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
    }
    
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SAMPLES, 16);

#ifdef __APPLE__    
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    std::string windowTitleBase = "Physically Based Rendering with OpenGL ";	
    
	if (s_bFullScreen)
	{
		int monitorsCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);
		const int activeMonitorIdx = 1;

		const GLFWvidmode* mode = glfwGetVideoMode(monitors[activeMonitorIdx]);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		window = glfwCreateWindow(mode->width, mode->height, windowTitleBase.data(), monitors[activeMonitorIdx], nullptr);
	}
	else
	{
		window = glfwCreateWindow(s_WindowWidth, s_WindowHeight, windowTitleBase.data(), nullptr, nullptr);
	}
    
    if (!window)
    {
        std::cerr << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, mouseMotionCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(s_bEnableVSync);

	glbinding::Binding::initialize(false);
    
    glfwGetFramebufferSize(window, &s_WindowWidth, &s_WindowHeight);
    reshape(window, s_WindowWidth, s_WindowHeight);

    init();
    
    auto openglVersion = glbinding::ContextInfo::version();
    windowTitleBase += std::to_string(openglVersion.majorVersion()) + "." + std::to_string(openglVersion.minorVersion());
    
    FPSTimer fpsTimer;
    Timer frameTimer;
    
    // Main loop
    while( !glfwWindowShouldClose(window) )
    {        
        draw();
        
        // Update animation
        scene->update(frameTimer.elapsedSeconds());
        
        frameTimer.start();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if(fpsTimer.update())
        {
            auto windowTitle = windowTitleBase + " - FPS: " + std::to_string(fpsTimer.getFPS());
            glfwSetWindowTitle(window, windowTitle.data());
        }
    }
    
    // Terminate GLFW
    glfwTerminate();
    
    // Exit program
    return EXIT_SUCCESS;
}
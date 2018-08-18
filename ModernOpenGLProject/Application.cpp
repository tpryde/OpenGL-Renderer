#include "stdafx.h"
#include "Application.h"


Application::Application (int Width, int Height, const char* Title, bool FullScreen) :
 m_WindowIsActive (true),
 m_FullscreenMode (FullScreen),
 m_WindowWidth (Width),
 m_WindowHeight (Height),
 m_WindowTitle (Title)
{
	Init ();
}


Application::~Application ()
{

	glfwDestroyWindow (m_window);

	glfwTerminate ();

}

void Application::Init ()
{
	InitWindow ();
	InitGraphics ();
	InitGameCore ();
}

void Application::InitWindow ()
{
	glfwInit ();

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);

	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 5);

	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWmonitor* monitor = nullptr;

	if (m_FullscreenMode)
	{
		monitor = glfwGetPrimaryMonitor ();
	}

	m_window = glfwCreateWindow (m_WindowWidth, m_WindowHeight, m_WindowTitle.c_str (), monitor, nullptr);

	if (!m_window)
	{
		throw std::runtime_error ("Failed to create window.");
	}

	glfwMakeContextCurrent (m_window);

	glfwSwapInterval (LIMIT_FPS);

	glfwSetWindowUserPointer (m_window, this);
	glfwSetFramebufferSizeCallback (m_window, FrameBufferSizeCallback);
	glfwSetKeyCallback (m_window, KeyCallback);
	glfwSetCursorPosCallback (m_window, MouseCallback);
	glfwSetWindowIconifyCallback (m_window, WindowIconifyCallback);

}

void Application::InitGraphics ()
{
	bool gladLoadedProperly = !gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress);

	if (gladLoadedProperly)
	{
		std::runtime_error ("Failed to initialize GLAD");
	}

	// During init, enable debug output
	glEnable (GL_DEBUG_OUTPUT);
	glDebugMessageCallback (GLMessageCallback, 0);

	ResizeGLScene (m_WindowWidth, m_WindowHeight);  // Set Up Our Perspective GL Screen
}

void Application::ResizeGLScene (GLsizei width, GLsizei height)
{
	if (height <= 0) height = 1;
	if (width <= 0) width = 1;

	m_WindowWidth = width;
	m_WindowHeight = height;

	if (m_GameCore)
	{
		m_GameCore->OnSurfaceChanged (width, height);
	}
		
}

void Application::InitGameCore ()
{
	m_GameCore = std::make_unique<GameCore> ();

	m_GameCore->OneTimeInit ();
	m_GameCore->OnSurfaceChanged (m_WindowWidth, m_WindowHeight);
}

void Application::Run ()
{
	bool done = false;

	auto previous_time = std::chrono::high_resolution_clock::now ();
	double lag = 0.0;

	while (!glfwWindowShouldClose (m_window) && !done)
	{
		auto current_time = std::chrono::high_resolution_clock::now ();

		std::chrono::duration<double, std::milli> elapsed = current_time - previous_time;
		previous_time = current_time;

		lag += elapsed.count ();

		glfwPollEvents ();

		ProcessInputs (done);

		while (lag >= MS_PER_UPDATE)
		{
			m_GameCore->Tick (MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}

		m_GameCore->OnDrawFrame ();

		glfwSwapBuffers (m_window);
	}

	m_GameCore = nullptr;
}

void Application::ProcessInputs (bool& ExitCondition)
{
	ExitCondition = m_KeyStates[GLFW_KEY_ESCAPE];
}

void APIENTRY Application::GLMessageCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	fprintf (stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void Application::WindowIconifyCallback (GLFWwindow* window, int iconified)
{
	auto ThisApp = reinterpret_cast<Application*> (glfwGetWindowUserPointer (window));

	assert (ThisApp);

	ThisApp->m_WindowIsActive = (iconified != 0);
}

void Application::FrameBufferSizeCallback (GLFWwindow* window, int width, int height)
{
	auto ThisApp = reinterpret_cast<Application*> (glfwGetWindowUserPointer (window));

	assert (ThisApp);

	ThisApp->ResizeGLScene (width, height);
}

void Application::KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto ThisApp = reinterpret_cast<Application*> (glfwGetWindowUserPointer (window));

	assert (ThisApp);

	switch (action)
	{
	case GLFW_PRESS:
		ThisApp->m_KeyStates[key] = true;
		break;
	case GLFW_RELEASE:
		ThisApp->m_KeyStates[key] = false;
		break;
	default:
		break;
	}
}

void Application::MouseCallback (GLFWwindow* window, double xpos, double ypos)
{
}
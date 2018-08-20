#include "stdafx.h"
#include "Application.h"


Application::Application (int Width, int Height, const char* Title, bool FullScreen) :
m_WinSize (Width, Height),
 m_WindowIsActive (true),
 m_FullscreenMode (FullScreen),
 m_WindowTitle (Title)
{
	Init ();
}


Application::~Application ()
{

	glfwDestroyWindow (m_Window);

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

	m_Window = glfwCreateWindow (m_WinSize.m_WidthPixel, m_WinSize.m_HeightPixel, m_WindowTitle.c_str (), monitor, nullptr);

	if (!m_Window)
	{
		throw std::runtime_error ("Failed to create window.");
	}

	glfwMakeContextCurrent (m_Window);

	glfwSwapInterval (LIMIT_FPS);

	glfwSetWindowUserPointer (m_Window, this);
	glfwSetFramebufferSizeCallback (m_Window, FrameBufferSizeCallback);
	glfwSetKeyCallback (m_Window, KeyCallback);
	glfwSetCursorPosCallback (m_Window, MouseMoveCallback);
	glfwSetMouseButtonCallback (m_Window, MouseButtonCallback);
	glfwSetWindowIconifyCallback (m_Window, WindowIconifyCallback);

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

	ResizeGLScene (m_WinSize.m_WidthPixel, m_WinSize.m_HeightPixel);  // Set Up Our Perspective GL Screen
}

void Application::ResizeGLScene (GLsizei width, GLsizei height)
{
	if (height <= 0) height = 1;
	if (width <= 0) width = 1;

	m_WinSize.m_WidthPixel = width;
	m_WinSize.m_HeightPixel = height;

	if (m_GameCore)
	{
		m_GameCore->OnSurfaceChanged (m_WinSize, width, height);
	}
		
}

void Application::InitGameCore ()
{
	m_GameCore = std::make_unique<GameCore> (m_WinSize);
}

void Application::Run ()
{
	bool done = false;

	auto previous_time = std::chrono::high_resolution_clock::now ();
	double lag = 0.0;

	while (!glfwWindowShouldClose (m_Window) && !done)
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

		glfwSwapBuffers (m_Window);
	}

	m_GameCore = nullptr;
}

void Application::ProcessInputs (bool& ExitCondition)
{
	ExitCondition = m_KeyStates[GLFW_KEY_ESCAPE];

}


std::string SeverityType (GLenum severity)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "Info";
	case GL_DEBUG_SEVERITY_LOW: return "Low";
	case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
	case GL_DEBUG_SEVERITY_HIGH: return "High";
	default: return "Severity Unknown";
	}
	
}

std::string MessageType (GLenum type)
{
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR: return "Error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behaviour";
	case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
	case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
	case GL_DEBUG_TYPE_MARKER: return "Marker";
	case GL_DEBUG_TYPE_PUSH_GROUP: return "Group Push";
	case GL_DEBUG_TYPE_POP_GROUP: return "Group Pop";
	case GL_DEBUG_TYPE_OTHER: return "Other";
	default: return "Unknown Message Type";
	}
}

void APIENTRY Application::GLMessageCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
	    " type = " << MessageType (type) <<
		", severity = " << SeverityType (severity) <<
		", message = " << message << std::endl;
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

void Application::MouseMoveCallback (GLFWwindow* window, double xpos, double ypos)
{
	auto ThisApp = reinterpret_cast<Application*> (glfwGetWindowUserPointer (window));
}

void Application::MouseButtonCallback (GLFWwindow* window, int button, int action, int mods)
{
	auto ThisApp = reinterpret_cast<Application*> (glfwGetWindowUserPointer (window));
	
}
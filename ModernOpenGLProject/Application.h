#pragma once

class Application
{
	GLFWwindow* m_window;
	std::unique_ptr<GameCore> m_GameCore;
	
	std::unordered_map<int, bool>  m_KeyStates;

	bool m_WindowIsActive = true;
	bool m_FullscreenMode = true;
	int m_WindowWidth = 0;
	int m_WindowHeight = 0;
	std::string m_WindowTitle;

	void Init ();
	void InitWindow ();
	void InitGraphics ();
	void InitGameCore ();

	void ResizeGLScene (GLsizei width, GLsizei height);


	static void APIENTRY GLMessageCallback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
	static void WindowIconifyCallback (GLFWwindow* window, int iconified);
	static void FrameBufferSizeCallback (GLFWwindow* window, int width, int height);
	static void KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback (GLFWwindow* window, double xpos, double ypos);

	void ProcessInputs (bool& ExitCondition);

public:
	Application (int Width, int Height, const char* Title, bool FullScreen = false);
	~Application ();

	void Run ();
};


#pragma once

struct WindowSize {
	float m_Near;
	float m_Far;
	int   m_WidthPixel;
	float m_Width;
	int   m_HeightPixel;
	float m_Height;
	bool  m_IsPerspective;

	WindowSize (int Width, int Height) : 
	m_Near (DEFAULT_NEAR), 
	m_Far (DEFAULT_FAR), 
	m_IsPerspective (false),
	m_WidthPixel (Width), m_Width (DEFAULT_BOUND),
	m_HeightPixel (Height), m_Height (DEFAULT_BOUND)
	{
	}

	glm::mat4 CreateOrtho () const
	{
		const auto HalfWidth = m_Width / 2.0f;
		const auto HalfHeight = m_Height / 2.0f;

		return glm::ortho (-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, m_Near, m_Far);
	}

	glm::mat4 CreatePerspective () const
	{
		const auto HalfWidth = m_Width / 2.0f;
		const auto HalfHeight = m_Height / 2.0f;

		return glm::frustum (-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, m_Near, m_Far);
	}

	glm::mat4 CreateProjectionMat () const
	{
		return m_IsPerspective ? CreatePerspective () : CreateOrtho ();
	}

	void AdjustAspectRatio (float width, float height)
	{
		const auto minDim = std::min (m_Width, m_Height);
			  
		const auto f_width = static_cast<float> (width);
		const auto f_height = static_cast<float> (height);

		if (width > height) {
			m_Width = minDim * f_width / f_height;
			m_Height = minDim;
		}
		else {
			m_Width = minDim;
			m_Height = minDim * f_height / f_width;
		}
	}
};

class Application
{
	GLFWwindow* m_Window;
	WindowSize m_WinSize;

	std::unique_ptr<GameCore> m_GameCore;
	
	std::unordered_map<int, bool>  m_KeyStates;

	bool m_WindowIsActive = true;
	bool m_FullscreenMode = true;
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


#include "stdafx.h"
#include "Application.h"

GameCore::GameCore (WindowSize& WinSize) :
	m_ProjMat (1.0f),
	m_ViewMat (1.0f),
	m_IsStaleView (false)
{
	OnSurfaceChanged (WinSize, WinSize.m_WidthPixel, WinSize.m_HeightPixel);

	OneTimeInit ();
}

GameCore::~GameCore()
{
    CheckForGLErrors();
}

void GameCore::OneTimeInit()
{
	glEnable (GL_DEPTH_TEST);

	m_ViewMat = glm::lookAt (glm::vec3 (0.0f, 0.0f, -20.0f),
							 glm::vec3 (0.0f, 0.0f, 0.0f),
							 glm::vec3 (0.0f, 1.0f, 0.0f)
							);
							
	m_GameGrid = std::make_unique<AutomataGrid> (10, m_ProjMat, m_ViewMat); // 20 * 20 m_Grid
}

void GameCore::Tick(double TimePassed)
{

}

void GameCore::OnSurfaceChanged(WindowSize& WinSize, int width, int height)
{
	WinSize.AdjustAspectRatio (width, height);

	m_ProjMat = WinSize.CreateProjectionMat ();

	WinSize.m_WidthPixel = width;
	WinSize.m_HeightPixel = height;

	glViewport(0, 0, width, height);

	if (m_GameGrid) m_GameGrid->Resize (m_ProjMat);
    CheckForGLErrors();
}


void GameCore::OnDrawFrame()
{
    // check for errors before we start drawing
    CheckForGLErrors();

    // clear the screen to darkish blue
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	m_GameGrid->Render (m_IsStaleView ? &m_ViewMat : nullptr);

	// check for errors again when we're done drawing
    CheckForGLErrors();
}
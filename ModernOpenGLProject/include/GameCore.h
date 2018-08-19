#pragma once
#include "AutomataGrid.h"

struct WindowSize;

class GameCore
{
public:
   
	std::unique_ptr<AutomataGrid> m_GameGrid;

	glm::mat4 m_ProjMat;
	glm::mat4 m_ViewMat;

	bool m_IsStaleView; //Set to true whenever view is changed

public:
    explicit GameCore(WindowSize& WinSize);
    ~GameCore();

    void OneTimeInit();
    void Tick(double TimePassed);

    void OnSurfaceChanged(WindowSize& WinSize, int width, int height);

    void OnDrawFrame();

};

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <assert.h>

#include <stdio.h>
#include <tchar.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <chrono>

#include "Helpers.h"

#include "ShaderProgram.h"

#include "GameCore.h"


const int LIMIT_FPS = 0;       //0:Unlimited, 1:60FPS, 2:30FPS
const double MS_PER_UPDATE = 1000.0 / 30.0;
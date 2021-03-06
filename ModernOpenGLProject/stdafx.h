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
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <algorithm>

#include "Helpers.h"

#include "ShaderProgram.h"

#include "GameCore.h"


const int LIMIT_FPS = 0;       //0:Unlimited, 1:60FPS, 2:30FPS
const double MS_PER_UPDATE = 1000.0 / 30.0;
const float DEFAULT_BOUND = 20.0f; //defines the total length of the viewport
const float DEFAULT_PADDING = 0.5f;
const float DEFAULT_OFFSET = 0.1f; //defines the total length of the viewport
const float DEFAULT_NEAR = 1.0f;
const float DEFAULT_FAR = 400.0f;
const int GRID_SIZE = 100;
const auto SECONDS_PER_CYCLE = std::chrono::duration<double, std::milli> (std::chrono::milliseconds (250)).count ();

const auto ALIVE = glm::vec3 (1.0f, 0.0f, 0.0f);
const auto DEAD  = glm::vec3 (1.0f, 1.0f, 1.0f);
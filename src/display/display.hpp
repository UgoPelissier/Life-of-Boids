#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <random>

#include "../common.hpp"
#include "glx/glx.hpp"
#include "shaders/lines.hpp"
#include "shaders/points.hpp"
#include "shaders/triangle.hpp"

static double xpos, ypos;

static bool add(false);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);
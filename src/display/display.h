#pragma once

#include "../config/config.h"
#include "../movement/agent.hpp"
#include "../movement/obstacle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <random>
#include "glx.hpp"
#include "triangle.hpp"

void error_callback(int error, const char* description);

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

vec2 scale(Agent& agent);

vec2 scale(Agent& agent, Real ratio);

vec2 scale(Obstacle& obstacle);

std::tuple<GLFWwindow*, VertexArray, VertexArray, Buffer, ShaderProgram, GLint> initWindow();

std::tuple <agents_t, agents_t, std::vector<Obstacle>, std::vector<std::array<triangle::Vertex, 3>>, std::vector<std::array<triangle::Vertex, 3>> > initAgentWindow();

void updateAgentWindow(GLFWwindow* window, agents_t& birds, agents_t& predators,  std::vector<Obstacle>& obstacles, std::vector<std::array<triangle::Vertex, 3>>& triangles);

void addAgent(GLFWwindow* window, bool& addBird, bool& addPredator, agents_t& birds, agents_t& predators, std::vector<Obstacle>& obstacles, std::vector<std::array<triangle::Vertex, 3>>& triangles);

void updateWindow(GLFWwindow* window,
                  std::vector<std::array<triangle::Vertex, 3>>& triangles,
                  std::vector<std::array<triangle::Vertex, 3>>& trianglesObs,
                  VertexArray& triangle_vertexArray,
                  VertexArray& triangleObs_vertexArray,
                  Buffer& triangle_buffer,
                  ShaderProgram& triangle_shaderProgram,
                  GLint& mvp_location);

void endWindow(GLFWwindow* window);
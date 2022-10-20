#pragma once

#include "../config/config.h"
#include "../movement/agent.hpp"
#include "../movement/obstacle.h"
#include "../movement/fruit.h"
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
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>


void error_callback(int error, const char* description);

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

vec2 scale(Agent& agent);

vec2 scale(Agent& agent, Real ratio);

vec2 scale(Obstacle& obstacle);

vec2 scale(FruitTree& tree);

vec2 scale(Fruit& fruit);

std::tuple<GLFWwindow*, VertexArray, VertexArray, VertexArray, VertexArray, Buffer, ShaderProgram, GLint> initWindow();

std::tuple<std::vector<Agent>, std::vector<Obstacle>, std::vector<FruitTree>, std::vector<Fruit>, std::vector<std::array<triangle::Vertex, 3>>, std::vector<std::array<triangle::Vertex, 3>>, std::vector<std::array<triangle::Vertex, 3>>, std::vector<std::array<triangle::Vertex, 3>>> initAgentWindow();

void updateAgentWindow(GLFWwindow* window, std::vector<Agent>& agents, std::vector<Obstacle>& obstacles, std::vector<FruitTree>& trees, std::vector<Fruit>& fruits, std::vector<std::array<triangle::Vertex, 3>>& triangles, std::vector<std::array<triangle::Vertex, 3>>& trianglesFruit);

void addAgent(GLFWwindow* window, bool& addBird, bool& addPredator, std::vector<Agent>& agents, std::vector<Obstacle>& obstacles, std::vector<std::array<triangle::Vertex, 3>>& triangles);

void updateWindow(GLFWwindow* window,
                  std::vector<std::array<triangle::Vertex, 3>>& triangles,
                  std::vector<std::array<triangle::Vertex, 3>>& trianglesObs,
                  std::vector<std::array<triangle::Vertex, 3>>& trianglesTree,
                  std::vector<std::array<triangle::Vertex, 3>>& trianglesFruit,
                  VertexArray& triangle_vertexArray,
                  VertexArray& triangleObs_vertexArray,
                  VertexArray& triangleTree_vertexArray,
                  VertexArray& triangleFruit_vertexArray,
                  Buffer& triangle_buffer,
                  ShaderProgram& triangle_shaderProgram,
                  GLint& mvp_location);

void display_fps(GLFWwindow* window, std::chrono::time_point<std::chrono::high_resolution_clock>& start, std::chrono::time_point<std::chrono::high_resolution_clock>& end);

void endWindow(GLFWwindow* window);
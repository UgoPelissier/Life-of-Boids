#pragma once

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

#include "../config/config.h"
#include "agents/agent.h"
#include "agents/bird.h"
#include "obstacles/obstacle.h"
#include "fruits/fruit.h"
#include "fruits/tree.h"

void error_callback(int error, const char* description);

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);

vec2 scale(Agent const& agent);

vec2 scale(Agent const& agent, Real ratio);

vec2 scale(Obstacle const& obstacle);

vec2 scale(Tree const& tree);

vec2 scale(Fruit const& fruit);

std::tuple <
        std::vector<Bird>,
        std::vector<Fruit>
        > updateObjects(std::vector<Obstacle>& obstacles,
              std::vector<Agent>& predators,
              std::vector<Bird>& birds,
              std::vector<Tree>& trees,
              std::vector<Fruit>& fruits);

std::tuple<GLFWwindow*, VertexArray, VertexArray, VertexArray, VertexArray, VertexArray, Buffer, ShaderProgram, GLint> initWindow();

std::tuple<
        std::vector<Obstacle>,
                std::vector<Agent>,
                        std::vector<Bird>,
                                std::vector<Tree>,
                                        std::vector<Fruit>,
                                            std::vector<std::array<triangle::Vertex, 3>>,
                                            std::vector<std::array<triangle::Vertex, 3>>,
                                            std::vector<std::array<triangle::Vertex, 3>>,
                                            std::vector<std::array<triangle::Vertex, 3>>,
                                            std::vector<std::array<triangle::Vertex, 3>>
                                            >
                                            initAgentWindow();

void updateAgentWindow(
        GLFWwindow* window,
        std::vector<Obstacle>& obstacles,
        std::vector<Agent>& predators,
        std::vector<Bird>& birds,
        std::vector<Tree>& trees,
        std::vector<Fruit>& fruits,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesPredators,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesBirds,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesFruit
        );

void addAgent(
        GLFWwindow* window,
        bool& addBird,
        bool& addPredator,
        std::vector<Obstacle>& obstacles,
        std::vector<Agent>& predators,
        std::vector<Bird>& birds,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesPredators,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesBirds
        );

void updateWindow(
        GLFWwindow* window,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesObs,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesPredators,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesBirds,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesTree,
        std::vector<std::array<triangle::Vertex, 3>>& trianglesFruit,
        VertexArray& triangleObs_vertexArray,
        VertexArray& trianglePred_vertexArray,
        VertexArray& triangleBird_vertexArray,
        VertexArray& triangleTree_vertexArray,
        VertexArray& triangleFruit_vertexArray,
        Buffer& triangle_buffer,
        ShaderProgram& triangle_shaderProgram,
        GLint& mvp_location
        );

void display_fps(
        GLFWwindow* window,
        std::chrono::time_point<std::chrono::high_resolution_clock>& start,
        std::chrono::time_point<std::chrono::high_resolution_clock>& end
        );

void endWindow(GLFWwindow* window);
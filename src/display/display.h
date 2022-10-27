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

#include "config.h"
#include "agent.h"
#include "bird.h"
#include "obstacle.h"
#include "fruit.h"
#include "tree.h"


using triangle_vertices_t = std::vector<std::array<triangle::Vertex, 3>>;

void error_callback(int error, const char* description);

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);


std::vector<Fruit> updateObjects(std::vector<Obstacle>& obstacles,
              agents_t& predators,
              birds_t& birds,
              std::vector<Tree>& trees,
              std::vector<Fruit>& fruits);

std::tuple<GLFWwindow*, VertexArray, VertexArray, VertexArray, VertexArray, VertexArray, Buffer, ShaderProgram, GLint> initWindow();

std::tuple<
    std::vector<Obstacle>,
    agents_t,
    birds_t,
    std::vector<Tree>,
    std::vector<Fruit>,
    triangle_vertices_t,
    triangle_vertices_t,
    triangle_vertices_t,
    triangle_vertices_t,
    triangle_vertices_t
> initAgentWindow();

void updateAgentWindow(
        GLFWwindow* window,
        std::vector<Obstacle>& obstacles,
        agents_t& predators,
        birds_t& birds,
        std::vector<Tree>& trees,
        std::vector<Fruit>& fruits,
        triangle_vertices_t& trianglesPredators,
        triangle_vertices_t& trianglesBirds,
        triangle_vertices_t& trianglesFruit
        );

void addAgent(
        GLFWwindow* window,
        bool& addBird,
        bool& addPredator,
        std::vector<Obstacle>& obstacles,
        agents_t& predators,
        birds_t& birds,
        triangle_vertices_t& trianglesPredators,
        triangle_vertices_t& trianglesBirds
        );

void updateWindow(
        GLFWwindow* window,
        triangle_vertices_t& trianglesObs,
        triangle_vertices_t& trianglesPredators,
        triangle_vertices_t& trianglesBirds,
        triangle_vertices_t& trianglesTree,
        triangle_vertices_t& trianglesFruit,
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
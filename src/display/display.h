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

#include "agent.h"
#include "predator.h"
#include "bird.h"
#include "config.h"
#include "obstacle.h"
#include "fruit.h"
#include "tree.h"
#include "main.h"


void error_callback(int error, const char* description);

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/);


std::vector<Fruit> updateObjects(std::vector<Obstacle>& obstacles,
              predators_t& predators,
              birds_t& birds,
              std::vector<Tree>& trees,
              std::vector<Fruit>& fruits);

std::tuple<GLFWwindow*, VertexArray, VertexArray, VertexArray, VertexArray, VertexArray, Buffer, ShaderProgram, GLint> initWindow();

std::tuple<
    std::vector<Obstacle>,
    predators_t,
    birds_t,
    std::vector<Tree>,
    std::vector<Fruit>,
    triangle::vertices_t,
    triangle::vertices_t,
    triangle::vertices_t,
    triangle::vertices_t,
    triangle::vertices_t
> initAgentWindow();

void updateAgentWindow(
        GLFWwindow* window,
        std::vector<Obstacle>& obstacles,
        predators_t& predators,
        birds_t& birds,
        std::vector<Tree>& trees,
        std::vector<Fruit>& fruits,
        triangle::vertices_t& trianglesPredators,
        triangle::vertices_t& trianglesBirds,
        triangle::vertices_t& trianglesFruit
        );

void addAgent(
        GLFWwindow* window,
        std::vector<Obstacle>& obstacles,
        predators_t& predators,
        birds_t& birds,
        triangle::vertices_t& trianglesPredators,
        triangle::vertices_t& trianglesBirds
        );

void updateWindow(
        GLFWwindow* window,
        triangle::vertices_t& trianglesObs,
        triangle::vertices_t& trianglesPredators,
        triangle::vertices_t& trianglesBirds,
        triangle::vertices_t& trianglesTree,
        triangle::vertices_t& trianglesFruit,
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
        std::chrono::time_point<std::chrono::high_resolution_clock>& end,
        double &total_fps
        );

void endWindow(GLFWwindow* window);
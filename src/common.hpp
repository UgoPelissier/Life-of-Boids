#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat2x2 = std::array<vec2, 2>;
using mat3x3 = std::array<vec3, 3>;
using mat4x4 = std::array<vec4, 4>;

//====================DEFAULT PARAMETERS DEFINED==================
#define DEFAULT_NUM_AGENTS 10

#define ANGLE_MIN -135
#define ANGLE_MAX 135

#define SPEED_MIN 0
#define SPEED_MAX 1

#define DEFAULT_COHESION_WEIGHT 0.5
#define DEFAULT_SEPARATION_WEIGHT 0.5 // replace with (1 - DEFAULT_COHESION_WEIGHT)

#define WIDTH 1200
#define HEIGHT 1000 // Window size parameter

#define TRIANGLE_SIZE    0.025
#define PI  3.14159265358979323846
//====================DEFAULT PARAMETERS DEFINED END==================

//====================USEFUL FUNCTIONS================================

double distance(vec2 p1, vec2 p2);
bool overlap(vec2 c, std::vector<vec2> centers);
bool outside_window(vec2 center, float ratio);
//====================USEFUL FUNCTIONS END============================

class Agent {
private:
    int m_x;
    int m_y;
    double m_angle;
    double m_velocity;
public:
	Agent(int x, int y, double angle, double velocity);
<<<<<<< HEAD
	bool update_vector(int x, int y, double angle, double velocity);
	int& X(); // get x by reference
	int& Y(); // get y by reference
	double& theta(); // get angle by reference
	double& speed(); // get speed by reference
	void move_next_pos()
=======

    int& get_x();
    int& get_y();
    double& get_angle();

<<<<<<< HEAD
    void update();
>>>>>>> thomas2
=======
    void update(int x, int y);
>>>>>>> d4937db52f7e71e81891a858a2fd228dcbf8c4e1
};

std::vector<Agent> initialiaze_agents();

void update_agents(std::vector<Agent>& agents);
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
#define DEFAULT_NUM_AGENTS 20

#define WIDTH 1200
#define HEIGHT 1000 // Window size parameter

#define CLOSE 100 // Border parameter

#define SPEED 1

#define TRIANGLE_SIZE    0.05
#define PI  3.14159265358979323846

//====================AGENT CLASS============================
class Agent {
private:
    int m_x;
    int m_y;
    double m_angle;
    double m_velocity;
public:
	Agent(int x, int y, double angle, double velocity);

    int& get_x();
    int& get_y();
    double& get_angle();

    std::pair<bool, double> borders();

    void borderUpdate(double opp);
    void update(int x, int y, double angle);
};

inline double modulo(double a, double b)
{
    double mod(a);
    if (a < 0) {
        while (mod < 0)
            mod += b;
    }
    else {
        while (mod >= b)
            mod -= b;
    }
    return mod;
}

std::vector<Agent> initialiaze_agents();
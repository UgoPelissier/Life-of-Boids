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
#define DEFAULT_NUM_AGENTS 150
#define DEFAULT_TYPE_AGENTS 2

#define ANGLE_MIN -135
#define ANGLE_MAX 135

#define SPEED_MIN 0
#define SPEED_MAX 3

#define WIDTH 2500
#define HEIGHT 2000 // Window size parameter

#define CLOSE 100 // Border parameter

#define SPEED 2

#define TRIANGLE_SIZE    0.05
#define PI  3.14159265358979323846

#define RANGE_SEPARATION 50
#define RANGE_ALIGMENT_LOW 50
#define RANGE_ALIGMENT_HIGH 100
#define RANGE_COHESION_LOW 80
#define RANGE_COHESION_HIGH 150
#define SPEED 1
#define WEIGHT_COHESION 0.25
#define WEIGHT_SEPARATION 0.5
#define WEIGHT_ALIGMENT 0.25
#define RELAXATION 0.1
//====================DEFAULT PARAMETERS DEFINED END==================

//====================USEFUL FUNCTIONS================================

double distance(vec2 p1, vec2 p2);
bool overlap(vec2 c, std::vector<vec2> centers);
bool outside_window(vec2 center, float ratio);
//====================USEFUL FUNCTIONS END============================


//====================AGENT CLASS============================
class Agent {
private:
    int m_x;
    int m_y;
    double m_angle;
    double m_velocity;
    int m_type; //type of the agent (agent 1 or agent 2 with different behavior)
    int m_cohesion_high;
    int m_cohesion_low;
    int m_separation;
    int m_aligment_high;
    int m_aligment_low;
    time_t m_time;
    int m_prey;

public:
	Agent(int x, int y, double angle, double velocity, int type, int cohesion_high, int cohesion_low, int separation, int aligment_high, int aligment_low, std::time_t time, int prey);

    int& get_x();
    int& get_y();
    double& get_angle();
    double& get_velocity();
    int& get_type();
    int& get_cohesion_high();
    int& get_cohesion_low();
    int& get_separation();
    int& get_aligment_high();
    int& get_aligment_low();
    std::time_t& get_time();
    int& get_prey();

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
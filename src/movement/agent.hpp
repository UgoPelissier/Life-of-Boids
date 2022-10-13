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
using mat4x4 = std::array<vec4, 4>;

//====================DEFAULT PARAMETERS==================
// Agent parameters
#define DEFAULT_NUM_AGENTS 10
#define DEFAULT_NUM_PREDATORS 1
#define CLOSE 100
#define SPEED 2
#define PI  3.14159265358979323846

// Display parameters
#define WIDTH 1400
#define HEIGHT 1000
#define TRIANGLE_SIZE   0.025

// Laws parameters
static double const SEPARATION = TRIANGLE_SIZE*WIDTH;
static double const ALIGNMENT = 2*SEPARATION;
#define PREDATOR 100
#define COHESION 200

#define SMOOTH_ALIGNMENT 0.25
#define SMOOTH_COHESION 0.5

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

inline vec2 normVector(vec2 v) {
    float norm = sqrt(v[0]*v[0] + v[1]*v[1]);
    return {v[0]/norm,v[1]/norm};
}


//====================AGENT CLASS============================
class Agent {
private:
    double m_x;
    double m_y;
    double m_angle;
    bool m_predator;

public:
    Agent();
	Agent(double x, double y, double angle, bool predator);

    double& get_x();
    double& get_y();
    double& get_angle();
    bool& get_predator();

    double distance(Agent a);
    std::vector<std::vector<size_t>> neighbours(size_t index, std::vector<Agent> agents);
    std::vector<size_t> predatorNeighbours(size_t index, std::vector<Agent> agents);
    size_t closerAgent(size_t index, std::vector<Agent> agents);

    bool equal(Agent a);
    bool overlap(Agent a);
    bool overlap(std::vector<Agent> agents);

    std::pair<bool, double> borders();

    void borderUpdate(double opp);
    void windowUpdate();
    void constantUpdate();

    vec3 center(std::vector<Agent> agents, std::vector<size_t> neighbours);
    vec3 centerSeparation(std::vector<Agent> agents, std::vector<size_t> neighbours);
    void cohesionLaw(std::vector<Agent> agents, std::vector<size_t> neighbours);
    void alignmentLaw(std::vector<Agent> agents, std::vector<size_t> neighbours);
    void separationLaw(std::vector<Agent> agents, std::vector<size_t> neighbours);
    void predatorLaw(size_t index, std::vector<Agent> agents);

    void updateAgent(size_t index, std::vector<Agent> agents);

};

//====================AGENT VECTORS FOR GLOBAL MOTION============================
std::vector<Agent> initialiaze_agents();

void updateAgents(std::vector<Agent>& agents);
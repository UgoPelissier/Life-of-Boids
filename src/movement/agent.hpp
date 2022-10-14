#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include "../main.h"

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat2x2 = std::array<vec2, 2>;
using mat4x4 = std::array<vec4, 4>;

//====================DEFAULT PARAMETERS==================
// Agent parameters
static size_t const DEFAULT_NUM_AGENTS = 200;
static size_t const DEFAULT_NUM_PREDATORS = 2;
static double const CLOSE = 50;
static double const SPEED = 2;
static double const PI  = 3.14159265358979323846;

// Display parameters
static const int WIDTH = 1800;
static const int HEIGHT = 1200;
static const float RATIO = (float)WIDTH / (float)HEIGHT;
static const double TRIANGLE_SIZE = 0.015;

// Laws parameters
static double const SEPARATION = TRIANGLE_SIZE*WIDTH;
static double const ALIGNMENT = 2*SEPARATION;
static double const PREDATOR = 50;
static double const COHESION = 200;

static double const SMOOTH_ALIGNMENT = 0.25;
static double const SMOOTH_COHESION = 0.5;

static bool const TURN_AROUND = false;

inline double modulo(double const& a, double const& b)
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

inline vec2 normVector(vec2 const& v) {
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
    bool m_border;
    double m_opp;

public:
    Agent();
	Agent(double x, double y, double angle, bool predator);

    double& get_x();
    double& get_y();
    double& get_angle();
    bool& get_predator();
    bool& get_border();
    double& get_opp();

    double distance(Agent a);
    std::vector<std::vector<size_t>> neighbours(size_t& index, std::vector<Agent>& agents);
    std::vector<size_t> predatorNeighbours(size_t& index, std::vector<Agent>& agents);
    size_t closerAgent(size_t& index, std::vector<Agent>& agents);

    bool equal(Agent& a);
    bool overlap(Agent const& a);
    bool overlap(std::vector<Agent>& agents);

    void borders();
    void borderNeighbour(size_t  const& index, std::vector<Agent> agents);

    void borderUpdate(double const& opp);
    void windowUpdate();
    void constantUpdate();

    vec3 center(std::vector<Agent> agents, std::vector<size_t> neighbours);
    vec3 centerSeparation(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void cohesionLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void alignmentLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void separationLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void predatorLaw(size_t& index, std::vector<Agent>& agents);

    void updateAgent(size_t& index, std::vector<Agent>& agents);

};

//====================AGENT VECTORS FOR GLOBAL MOTION============================
std::vector<Agent> initialiaze_agents();

void checkBorders(std::vector<Agent>& agents);

void updateAgents(std::vector<Agent>& agents);
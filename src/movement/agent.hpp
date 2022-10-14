#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include "../main.h"
#include "../config/config.h"
#include "obstacle.h"

//====================AGENT CLASS============================
class Agent {
private:
    double m_x;
    double m_y;
    double m_angle;
    bool m_predator;
    bool m_obstacle;

public:
    Agent();
    Agent(double const& x, double const& y);
	Agent(double const& x, double const& y, double const& angle, bool const& predator);

    double& get_x();
    double& get_y();
    double& get_angle();
    bool& get_predator();
    bool& get_obstacle();

    double distance(Agent a);
    std::vector<std::vector<size_t>> neighbours(size_t const& index, std::vector<Agent>& agents);
    std::vector<size_t> predatorNeighbours(size_t const& index, std::vector<Agent>& agents);
    size_t closerAgent(size_t const& index, std::vector<Agent>& agents);

    bool equal(Agent& a);
    bool overlap(Agent const& a);
    bool overlap(std::vector<Agent>& agents);

    std::vector<size_t> obstacle(std::vector<Obstacle>& obstacles);

    void windowUpdate();
    void constantUpdate();

    vec3 center(std::vector<Agent> agents, std::vector<size_t>& neighbours);
    vec3 centerSeparation(std::vector<Agent>& agents, std::vector<size_t>& neighbours);

    void cohesionLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void alignmentLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void separationLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void predatorLaw(size_t& index, std::vector<Agent>& agents);
    void obstacleLaw(std::vector<Obstacle>& obstacles, std::vector<size_t>& neighboursObstacles);

    void updateAgent(size_t& index, std::vector<Agent>& agents, std::vector<Obstacle>& obstacles);

};

//====================AGENT VECTORS FOR GLOBAL MOTION============================
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

std::vector<Agent> initialiaze_agents(std::vector<Obstacle>& obstacles);

void checkObstacles(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles);

void updateAgents(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles);
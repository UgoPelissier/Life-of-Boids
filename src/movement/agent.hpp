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
    Real m_x;
    Real m_y;
    Real m_angle;
    bool m_predator;
    bool m_obstacle;
    bool m_alive;
    size_t m_index;

public:
    Agent();
    Agent(Real const& x, Real const& y);
    Agent(Real const& x, Real const& y, Real const& angle, bool const& predator);
	Agent(Real const& x, Real const& y, Real const& angle, bool const& predator, size_t& index);

    Real& get_x();
    Real& get_y();
    Real& get_angle();
    bool& get_predator();
    bool& get_obstacle();
    bool& get_alive();
    size_t& get_index();

    Real distance(Agent a);
    Real angle (Agent& a);
    bool insideFieldView(Agent& a);
    std::vector<std::vector<size_t>> neighbours(std::vector<Agent>& agents);
    std::vector<size_t> predatorNeighbours(std::vector<Agent>& agents);
    size_t closestAgent(std::vector<Agent>& agents);

    bool operator==(Agent& a) const;
    bool overlap(Agent& a);
    bool overlap(std::vector<Agent>& agents);

    std::vector<size_t> obstacle(std::vector<Obstacle>& obstacles);

    void windowUpdate();
    void constantUpdate();

    vec3 center(std::vector<Agent> agents, std::vector<size_t>& neighbours);
    vec3 centerSeparation(std::vector<Agent>& agents, std::vector<size_t>& neighbours);

    void cohesionLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void alignmentLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void separationLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours);
    void predatorLaw(std::vector<Agent>& agents);
    void obstacleLaw(std::vector<Obstacle>& obstacles, std::vector<size_t>& neighboursObstacles);



    void updateAgent(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles);

};

//====================AGENT VECTORS FOR GLOBAL MOTION============================
inline Real modulo(Real const& a, Real const& b)
{
    Real mod(a);
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

inline Real angleVector(vec2 v1, vec2 v2) {
    Real dot = v1[0]*v2[0] + v1[1]*v2[1];
    Real det = v1[0]*v2[1] - v1[1]*v2[0];
    Real angle = atan2(det, dot);
    return angle;
}

std::vector<Agent> initialiaze_agents(std::vector<Obstacle>& obstacles);

std::vector<Agent> updateAgents(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles);
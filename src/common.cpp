#include "common.hpp"

//====================USEFUL FUNCTIONS================================
using vec2 = std::array<float, 2>;

double distance(vec2 p1, vec2 p2) {
    double d(0);
    d = std::sqrt( (p2[0]-p1[0])*(p2[0]-p1[0]) +  (p2[1]-p1[1])*(p2[1]-p1[1]) );
    return d;
}

bool overlap(vec2 c, std::vector<vec2> centers) {
    float h = TRIANGLE_SIZE * std::sqrt(3) / 2;
    float l = 2*h/3;
    for (auto& center : centers) {
        if (distance(c, center)<2*l) {
            return true;
        }
    }
    return false;
}

bool outside_window(vec2 center, float ratio) {
    float h = TRIANGLE_SIZE * std::sqrt(3) / 2;
    float l = 2* h / 3;
    return (bool)(std::abs(center[0]) > ratio - l || std::abs(center[1]) > 1 - l);
}

std::vector<Agent> initialiaze_agents() {

    std::vector<Agent> agents;

    int randomX;
    int randomY;
    double randomAngle;
    double randomVelocity;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::mt19937_64 rng;

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomX = uniX(rng);
        randomY = uniY(rng); 
        randomAngle = 2 * PI * unif(rng);
        randomVelocity = unif(rng);
        if (i == 0) {
            randomX = (int) WIDTH / 2;
            randomY = (int) HEIGHT / 2;
            randomAngle = 2;
        }
        else {
            randomX = (int) WIDTH / 2 -200;
            randomY = (int) HEIGHT / 2 +200;
            randomAngle = 5.7;

        }  

        agents.push_back(Agent(randomX,randomY,randomAngle, randomVelocity));
    }

    return agents;
}
/*
void update_agents(std::vector<Agent>& agents) {
    
    int x, y;
    for (Agent& agent : agents) {
        // update x y here
        agent.update(x, y);
    }
}
*/
//====================USEFUL FUNCTIONS END============================

//===============Agent class functions definitions================

Agent::Agent(int x, int y, double angle, double velocity) {
    m_x = x;
    m_y = y;
    m_velocity = velocity;
    m_angle = angle;
}

int& Agent::get_x() {
    return m_x;
}

int& Agent::get_y() {
    return m_y;
}

double& Agent::get_angle() {
    return m_angle;
}

void Agent::update(int x, int y, double angle) {
    m_x = x;
    m_y = y;
    m_angle = angle;
}
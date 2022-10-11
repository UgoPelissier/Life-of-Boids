#include "common.hpp"

//====================USEFUL FUNCTIONS================================
using vec2 = std::array<float, 2>;

double distance(vec2 p1, vec2 p2) {
    double d(0);
    d = std::sqrt( (p2[0]-p1[0])*(p2[0]-p1[0]) +  (p2[1]-p1[1])*(p2[1]-p1[1]) );
    return d;
}

bool overlap(vec2 c, std::vector<vec2> centers) {
    float h = triangleSize*std::sqrt(3)/2;
    float l = 2*h/3;
    for (auto& center : centers) {
        if (distance(c, center)<2*l) {
            return true;
        }
    }
    return false;
}

bool outsideWindow(vec2 center, float ratio) {
    float h = triangleSize*std::sqrt(3)/2;
    float l = 2*h/3;
    if ( std::abs(center[0])>ratio-l or std::abs(center[1])>1-l ) {
        return true;
    }
    return false;
}

std::vector<Agent> initialiazeAgents() {

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
        randomAngle = 2*PI*unif(rng);
        randomVelocity = unif(rng);

        agents.push_back(Agent(randomX,randomY,randomAngle, randomVelocity));
    }

    return agents;
}

void updateAgents(std::vector<Agent>& agents) {
    for (Agent& agent : agents) {
        agent.update();
    }
}

//====================USEFUL FUNCTIONS END============================

//===============Agent class functions definitions================
Agent::Agent(int x, int y, double angle, double velocity) {
    m_x = x;
    m_y = y;
    m_velocity = velocity;
    m_angle = angle;
}

int& Agent::getX() {
    return m_x;
}

int& Agent::getY() {
    return m_y;
}

double& Agent::getAngle() {
    return m_angle;
}

void Agent::update() {
    m_x += 1;
    m_y += 1;
    m_angle += 1e-1;
}
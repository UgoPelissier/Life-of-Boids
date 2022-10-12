#include "common.hpp"

//====================AGENT CLASS============================
Agent::Agent(int x, int y, double angle, double velocity) {
    m_x = x;
    m_y = y;
    m_angle = angle;
    m_velocity = velocity;
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

std::pair<bool, double> Agent::borders(){
    bool tooClose(false);
    double opp(0);
    if ( m_x<CLOSE ) {
        tooClose = true;
        opp = 0;
    }
    if ( (WIDTH-CLOSE<m_x) && (m_x<WIDTH) ) {
        tooClose = true;
        opp = PI;
    }
    if ( m_y<CLOSE ) {
        tooClose = true;
        opp = PI/2;
    }
    if ( (HEIGHT-CLOSE<m_y) && (m_y<HEIGHT) ) {
        tooClose = true;
        opp = -PI/2;
    }
    return std::make_pair(tooClose, opp);
}

void Agent::borderUpdate(double opp) {
    if ( std::abs( opp - m_angle ) > PI/10 ) {
        m_angle = modulo(m_angle+PI-PI/25, 2*PI)-PI;
    }
    m_x += (int)round(m_velocity * cos(m_angle));
    m_y += (int)round(m_velocity * sin(m_angle));
}

void Agent::update(int x, int y, double angle) {
    m_x = x;
    m_y = y;
    m_angle = angle;
}

std::vector<Agent> initialiaze_agents() {

    std::vector<Agent> agents;
    Agent agent(0,0,0,0);

    int randomX;
    int randomY;
    double randomAngle;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::mt19937_64 rng;

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomX = uniX(rng);
        randomY = uniY(rng);
        randomAngle = 2*PI*unif(rng)-PI;

        agent = Agent(randomX,randomY,randomAngle, SPEED);
        while (agent.borders().first) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomAngle = 2*PI*unif(rng)-PI;

            agent = Agent(randomX,randomY,randomAngle, SPEED);
        }
        agents.push_back(agent);
    }
    return agents;
}
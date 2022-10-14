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
    Agent agent(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);

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
        agent = Agent(randomX, randomY, randomAngle, SPEED, 1, 0, 0, 0, 0, 0, 0, 0);
        while (agent.borders().first) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomAngle = 2 * PI * unif(rng) - PI;

            agent = Agent(randomX, randomY, randomAngle, SPEED, 1, 0, 0, 0, 0, 0, 0, 0);
        }

        if (i % 5 == 0) {
            agents.push_back(Agent(randomX, randomY, randomAngle, SPEED*2, 2, RANGE_COHESION_HIGH+50, RANGE_COHESION_LOW-50, RANGE_SEPARATION-20, RANGE_ALIGMENT_HIGH-50, RANGE_ALIGMENT_LOW-30, 0, 0));
        }
        else {
            agents.push_back(Agent(randomX, randomY, randomAngle, randomVelocity, 1, RANGE_COHESION_HIGH, RANGE_COHESION_LOW, RANGE_SEPARATION, RANGE_ALIGMENT_HIGH, RANGE_ALIGMENT_LOW, 0, 0));
        }

        
    }
    return agents;
}

//====================USEFUL FUNCTIONS END============================

//===============Agent class functions definitions================

Agent::Agent(int x, int y, double angle, double velocity, int type, int cohesion_high, int cohesion_low, int separation, int aligment_high, int aligment_low, std::time_t time, int prey) {
    m_type = type;
    m_cohesion_high = cohesion_high;
    m_cohesion_low = cohesion_low;
    m_separation = separation;
    m_aligment_high = aligment_high;
    m_aligment_low = aligment_low;
    m_velocity = velocity;
    m_angle = angle;
    m_x = x;
    m_y = y;
    m_time = time;
    m_prey = prey;

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

int& Agent::get_type() {
    return m_type;
}

int& Agent::get_cohesion_high() {
    return m_cohesion_high;
}

int& Agent::get_cohesion_low() {
    return m_cohesion_low;
}

int& Agent::get_separation() {
    return m_separation;
}

int& Agent::get_aligment_high() {
    return m_aligment_high;
}

int& Agent::get_aligment_low() {
    return m_aligment_low;
}

double& Agent::get_velocity() {
    return m_velocity;
}

std::time_t& Agent::get_time() {
    return m_time;
}

int& Agent::get_prey() {
    return m_prey;
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
/*
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
*/
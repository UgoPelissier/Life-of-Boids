#include "agent.hpp"

//====================AGENT CLASS============================
Agent::Agent() {
    m_x = 0;
    m_y = 0;
    m_angle = 0;
    m_predator = false;
    m_obstacle = false;
    m_alive = true;
}

Agent::Agent(Real const& x, Real const& y) {
    m_x = x;
    m_y = y;
    m_angle = 0;
    m_predator = false;
    m_obstacle = false;
    m_alive = true;
}

Agent::Agent(Real const& x, Real const& y, Real const& angle, bool const& predator) {
    m_x = x;
    m_y = y;
    m_angle = angle;
    m_predator = predator;
    m_obstacle = false;
    m_alive = true;
}

Real& Agent::get_x() {
    return m_x;
}

Real& Agent::get_y() {
    return m_y;
}

Real& Agent::get_angle() {
    return m_angle;
}

bool& Agent::get_predator() {
    return m_predator;
}

bool& Agent::get_obstacle() {
    return m_obstacle;
}

bool& Agent::get_alive() {
    return m_alive;
}

Real Agent::distance(Agent a) {
    return (Real)(std::sqrt((m_x - a.get_x()) * (m_x - a.get_x()) + (m_y - a.get_y()) * (m_y - a.get_y())));
}

Real Agent::angle(Agent& a) {
    vec2 v1 = {cos(m_angle),sin(m_angle)};
    vec2 v2 = normVector({a.get_x()-m_x,a.get_y()-m_y});
    Real angle = angleVector(v1,v2);
    return angle;
}

bool Agent::insideFieldView(Agent& a) {
    if (this->angle(a) > VIEW_RANGE/2)
        return false;
    return true;
}

std::vector<std::vector<size_t>> Agent::neighbours(size_t const& index, std::vector<Agent>& agents) {
    std::vector<size_t> predators, separation, alignment, cohesion;
    for (size_t i(0); i < agents.size(); i++) {
        if (index!=i && this->insideFieldView(agents[i])) {
            if ( !agents[i].get_predator() ) {
                if (this->distance(agents[i]) < SEPARATION_RANGE) {
                    separation.push_back(i);
                }
                else if (this->distance(agents[i]) > SEPARATION_RANGE && this->distance(agents[i]) < ALIGNMENT_RANGE) {
                    alignment.push_back(i);
                }
                else if (this->distance(agents[i]) > ALIGNMENT_RANGE && this->distance(agents[i]) < COHESION_RANGE) {
                        cohesion.push_back(i);
                    }
            }
            else {
                if (this->distance(agents[i]) < PREDATOR_RANGE) {
                    predators.push_back(i);
                    if (this->distance(agents[i]) < DEAD_RANGE) {
                        m_alive = false;
                    }
                }
            }
        }
    }
    return {predators, separation, alignment, cohesion};
}

std::vector<size_t> Agent::predatorNeighbours(size_t const& index, std::vector<Agent>& agents) {
    std::vector<size_t> predators;
    for (size_t i(0); i < agents.size(); i++) {
        if ( index!=i ) {
            if ( agents[i].get_predator() ) {
                if ( this->distance(agents[i]) < PREDATOR_RANGE ) {
                    predators.push_back(i);
                }
            }
        }
    }
    return predators;
}

size_t Agent::closerAgent(size_t const& index, std::vector<Agent>& agents) {
    Real d = WIDTH;
    size_t closer;
    for (size_t i(0); i < agents.size(); i++) {
        if (index!=i) {
            if ( !agents[i].get_predator() ) {
                if (this->distance(agents[i]) < d) {
                    d = distance(agents[i]);
                    closer = i;
                }
            }
        }
    }
    return closer;
}

bool Agent::equal(Agent& a) {
    if (m_x==a.get_x() && m_y==a.get_y() && m_angle==a.get_angle()) {
        return true;
    }
    return false;
}

bool Agent::overlap(Agent& a) {
    Real l = (sqrt(3)*BODY_SIZE/2)*(WIDTH/2);
    if (this->distance(a)<l) {
        return true;
    }
    return false;
}

bool Agent::overlap(std::vector<Agent>& agents) {
    for(Agent& agent : agents) {
        if ( (!this->equal(agent)) && this->overlap(agent) ) {
            return true;
        }
    }
    return false;
}

std::vector<size_t> Agent::obstacle(std::vector<Obstacle>& obstacles) {
    m_obstacle = false;
    std::vector<size_t> neighboursObstacles;
    for (size_t i(0); i < obstacles.size(); i++) {
        if ( this->distance(Agent(obstacles[i].get_x(),obstacles[i].get_y())) < std::max(obstacles[i].get_height()/1.25,obstacles[i].get_width()/1.25) ) {
            m_obstacle = true;
            neighboursObstacles.push_back(i);
        }
    }
    return neighboursObstacles;
}

void Agent::windowUpdate() {
    m_x = modulo(m_x,WIDTH);
    m_y = modulo(m_y, HEIGHT);
}

void Agent::constantUpdate() {
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

vec3 Agent::center(std::vector<Agent> agents, std::vector<size_t>& neighbours) {
    size_t n(neighbours.size());
    Real x(m_x), y(m_y), angle(m_angle);
    for(size_t& i : neighbours) {
        x += agents[i].get_x();
        y += agents[i].get_y();
        angle += agents[i].get_angle();
    }
    return {(float)(x/(n+1)), (float)(y/(n+1)), (float)(angle/(n+1))};
}

vec3 Agent::centerSeparation(std::vector<Agent>& agents, std::vector<size_t> & neighbours) {
    size_t n(neighbours.size());
    Real x(0), y(0), angle(0);
    for(size_t& i : neighbours) {
        x += agents[i].get_x();
        y += agents[i].get_y();
        angle += agents[i].get_angle();
    }
    return {(float)(x/n), (float)(y/n), (float)(angle/n)};
}

void Agent::cohesionLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours) {
    vec3 v = this->center(agents, neighbours);
    m_angle = (1-COHESION_RELAXATION)*atan2((v[1]-m_y),(v[0]-m_x)) + COHESION_RELAXATION*m_angle;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::alignmentLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours) {
    vec3 v = this->center(agents, neighbours);
    m_angle = m_angle = (1-ALIGNMENT_RELAXATION)*v[2] + ALIGNMENT_RELAXATION*m_angle ;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::separationLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours) {

    vec3 v = this->centerSeparation(agents, neighbours);
    vec2 separation = normVector({(float)(m_x-v[0]),(float)(m_y-v[1])});

    m_angle = (1-SEPARATION_RELAXATION)*atan2(separation[1],separation[0]) + SEPARATION_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::predatorLaw(size_t& index, std::vector<Agent>& agents) {

    size_t closer = this->closerAgent(index, agents);
    vec2 target = normVector({(float)(agents[closer].get_x() - m_x),(float)(agents[closer].get_y() - m_y)});

    m_angle = (1-PREDATOR_RELAXATION)*atan2(target[1],target[0]) + PREDATOR_RELAXATION*m_angle;

    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

void Agent::obstacleLaw(std::vector<Obstacle>& obstacles, std::vector<size_t>& neighboursObstacles) {

    std::vector<Agent> agents;
    for (size_t i = 0; i<obstacles.size(); i++) {
        agents.push_back(Agent(obstacles[i].get_x(),obstacles[i].get_y()));
    }

    vec3 v = this->centerSeparation(agents, neighboursObstacles);
    vec2 separation = normVector({(float)(m_x-v[0]),(float)(m_y-v[1])});

    m_angle = (1-OBSTACLE_RELAXATION)*atan2(separation[1],separation[0]) + OBSTACLE_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::updateAgent(size_t& index, std::vector<Agent>& agents, std::vector<Obstacle>& obstacles) {

    std::vector<std::vector<size_t>> v;
    std::vector<size_t> predators, separation, alignment, cohesion, neighboursObstacles;

    neighboursObstacles = this->obstacle(obstacles);
    v = this->neighbours(index, agents);

    if (m_predator)
        predators = this->predatorNeighbours(index, agents);
    else
        predators = v[0];

    separation = v[1];
    alignment = v[2];
    cohesion = v[3];

    if (m_predator) {
        if (m_obstacle) {
            this->obstacleLaw(obstacles, neighboursObstacles);
        } else {
            if (!predators.empty())
                this->separationLaw(agents, predators);
            else
                this->predatorLaw(index, agents);
        }
    }
    else {
        if (m_obstacle) {
            this->obstacleLaw(obstacles, neighboursObstacles);
        } else {
            if (!predators.empty()) {
                this->separationLaw(agents, predators);
            } else if (!separation.empty()) {
                this->separationLaw(agents, separation);
            } else if (!alignment.empty()) {
                this->alignmentLaw(agents, alignment);
            } else if (!cohesion.empty()) {
                this->cohesionLaw(agents, cohesion);
            } else {
                this->constantUpdate();
            }
        }
    }
    this->windowUpdate();
}

//====================RELATED FUNCTIONS============================
std::vector<Agent> initialiaze_agents(std::vector<Obstacle>& obstacles) {

    std::vector<Agent> agents;
    Agent agent;

    int randomX;
    int randomY;
    Real randomAngle;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::mt19937_64 rng;

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomX = uniX(rng);
        randomY = uniY(rng);
        randomAngle = 2*PI*unif(rng)-PI;

        agent = Agent(randomX,randomY,randomAngle,false);
        agent.obstacle(obstacles);
        while (agent.get_obstacle() || agent.overlap(agents)) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomAngle = 2*PI*unif(rng)-PI;

            agent = Agent(randomX,randomY,randomAngle,false);
            agent.obstacle(obstacles);
        }
        agents.push_back(agent);
    }
    for (size_t i = 0; i<DEFAULT_NUM_PREDATORS; ++i) {

        randomX = uniX(rng);
        randomY = uniY(rng);
        randomAngle = 2*PI*unif(rng)-PI;

        agent = Agent(randomX,randomY,randomAngle,true);
        agent.obstacle(obstacles);
        while (agent.get_obstacle() || agent.overlap(agents)) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomAngle = 2*PI*unif(rng)-PI;

            agent = Agent(randomX,randomY,randomAngle,true);
            agent.obstacle(obstacles);
        }
        agents.push_back(agent);
    }
    return agents;
}

void checkObstacles(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles) {
    for (size_t i(0); i < agents.size(); i++) {
        agents[i].obstacle(obstacles);
    }
}

std::vector<Agent> updateAgents(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles) {
    std::vector<Agent> newAgents;
    checkObstacles(agents, obstacles);
    for (size_t i(0); i < agents.size(); i++) {
        agents[i].updateAgent(i, agents, obstacles);
    }
    for (Agent& agent : agents) {
        if ( agent.get_predator() || agent.get_alive() )
            newAgents.push_back(agent);
    }
    return newAgents;
}
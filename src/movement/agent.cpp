#include "agent.hpp"

//====================AGENT CLASS============================
Agent::Agent() {
    m_x = 0;
    m_y = 0;
    m_angle = 0;
    m_predator = false;
    m_obstacle = false;
    m_alive = true;
    m_index = 0;
}

Agent::Agent(Real const& x, Real const& y) {
    m_x = x;
    m_y = y;
    m_angle = 0;
    m_predator = false;
    m_obstacle = false;
    m_alive = true;
    m_index = 0;
}

Agent::Agent(Real const& x, Real const& y, Real const& angle, bool const& predator) {
    m_x = x;
    m_y = y;
    m_angle = angle;
    m_predator = predator;
    m_obstacle = false;
    m_alive = true;
    m_index = 0;
}

Agent::Agent(Real const& x, Real const& y, Real const& angle, bool const& predator, size_t& index) {
    m_x = x;
    m_y = y;
    m_angle = angle;
    m_predator = predator;
    m_obstacle = false;
    m_alive = true;
    m_index = index;
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

size_t& Agent::get_index() {
    return m_index;
}

Real Agent::distance(Agent a) const {
    Real diff_x = (m_x - a.get_x());
    Real diff_y = (m_y - a.get_y());

    return std::sqrt((diff_x * diff_x) + (diff_y * diff_y));
}

Real Agent::distance(Obstacle obs) const {
    Real diff_x = (m_x - obs.get_x());
    Real diff_y = (m_y - obs.get_y());

    return std::sqrt((diff_x * diff_x) + (diff_y * diff_y));
}

Real Agent::angle(Agent& a) const {
    vec2 v1 = {(Real)cos(m_angle),(Real)sin(m_angle)};
    vec2 v2 = normVector({a.get_x()-m_x,a.get_y()-m_y});
    Real angle = angleVector(v1,v2);
    return angle;
}

bool Agent::insideFieldView(Agent& a) const {
    if (this->angle(a) > VIEW_RANGE/2)
        return false;
    return true;
}

std::vector<std::vector<size_t>> Agent::neighbours(std::vector<Agent>& agents) {

    std::vector<size_t> predators, separation, alignment, cohesion;
    Real current_distance;
    for (size_t i(0); i < agents.size(); i++) {
        if (m_index!=i && this->insideFieldView(agents[i])) {
            current_distance = this->distance(agents[i]);
            if ( !agents[i].get_predator() ) {
                if (current_distance < SEPARATION_RANGE) {
                    separation.push_back(i);
                }
                else if (current_distance > SEPARATION_RANGE && current_distance < ALIGNMENT_RANGE) {
                    alignment.push_back(i);
                }
                else if (current_distance > ALIGNMENT_RANGE && current_distance < COHESION_RANGE) {
                        cohesion.push_back(i);
                    }
            }
            else {
                if (current_distance < PREDATOR_RANGE) {
                    predators.push_back(i);
                    if (current_distance < DEAD_RANGE) {
                        m_alive = false;
                    }
                }
            }
        }
    }
    return {predators, separation, alignment, cohesion};
}

std::vector<size_t> Agent::predatorNeighbours(std::vector<Agent>& agents) const {
    std::vector<size_t> predators;
    for (size_t i(0); i < agents.size(); i++) {
        if ( m_index!=i ) {
            if ( agents[i].get_predator() ) {
                if ( this->distance(agents[i]) < PREDATOR_RANGE ) {
                    predators.push_back(i);
                }
            }
        }
    }
    return predators;
}

size_t Agent::closestAgent(std::vector<Agent>& agents) const {
    Real d = WIDTH;
    size_t closest_index = agents[0].get_index();

    for (size_t i(1); i < agents.size(); i++) {

        if (m_index != i) {
            Real current_distance = this->distance(agents[i]);
            if (!agents[i].get_predator() && current_distance < d) {
                d = current_distance;
                closest_index = i;
            }
        }
    }

    return closest_index;

}

bool Agent::operator==(Agent& a) const {
    return (m_x == a.get_x() && m_y == a.get_y() && m_angle == a.get_angle());
}

bool Agent::overlap(Agent& a) const {
    Real l = (sqrt(3)*BODY_SIZE/2)*(WIDTH/2);
    if (this->distance(a)<l) {
        return true;
    }
    return false;
}

bool Agent::overlap(std::vector<Agent>& agents) const {
    for(Agent& agent : agents) {
        if ( this->operator==(agent) && this->overlap(agent) ) {
            return true;
        }
    }
    return false;
}

std::vector<size_t> Agent::obstacle(std::vector<Obstacle>& obstacles) {
    m_obstacle = false;
    std::vector<size_t> neighboursObstacles;
    for (size_t i(0); i < obstacles.size(); i++) {
        if ( this->distance(obstacles[i]) < std::max(obstacles[i].get_height()/2.5,obstacles[i].get_width()/2.5) ) {
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

vec3 Agent::center(std::vector<Agent> agents, std::vector<size_t>& neighbours) const {
    size_t n(neighbours.size());
    Real x(m_x), y(m_y), angle(m_angle);
    Real inv = 1 / (n + 1);
    for(size_t& i : neighbours) {
        x += agents[i].get_x();
        y += agents[i].get_y();
        angle += agents[i].get_angle();
    }
    return {(Real) x * inv, (Real) y * inv, (Real) angle * inv};
}

vec3 Agent::centerSeparation(std::vector<Agent>& agents, std::vector<size_t> & neighbours) {
    size_t n(neighbours.size());
    Real x(0), y(0), angle(0);
    Real inv = 1 / n;
    for(size_t& i : neighbours) {
        x += agents[i].get_x();
        y += agents[i].get_y();
        angle += agents[i].get_angle();
    }
    return {(Real) x * inv, (Real) y * inv, (Real) angle * inv};
}

void Agent::cohesionLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours) {

    vec3 v = this->center(agents, neighbours);

    m_angle = (1-COHESION_RELAXATION)*atan2((v[1]-m_y),(v[0]-m_x)) + COHESION_RELAXATION*m_angle;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::alignmentLaw(std::vector<Agent>& agents, std::vector<size_t>& neighbours) {

    vec3 v = this->center(agents, neighbours);

    m_angle = (1-ALIGNMENT_RELAXATION)*v[2] + ALIGNMENT_RELAXATION*m_angle ;
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

void Agent::biSeparationLaw(std::vector<Agent>& agents, std::vector<size_t>& birdsNeighbours, std::vector<size_t>& predNeighbours) {

    vec3 vBird = this->centerSeparation(agents, birdsNeighbours);
    vec2 separationBird = normVector({(float)(m_x-vBird[0]),(float)(m_y-vBird[1])});

    vec3 vPred = this->centerSeparation(agents, predNeighbours);
    vec2 separationPred = normVector({(float)(m_x-vPred[0]),(float)(m_y-vPred[1])});

    Real angleBird = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;
    Real anglePred = (1-SEPARATION_RELAXATION)*atan2(separationPred[1],separationPred[0]) + SEPARATION_RELAXATION*m_angle;

    m_angle = 0.5*anglePred + 0.5*angleBird;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);

}

void Agent::predatorLaw(std::vector<Agent>& agents) {

    size_t closest_index = this->closestAgent(agents);
    vec2 target = normVector({(float)(agents[closest_index].get_x() - m_x),(float)(agents[closest_index].get_y() - m_y)});

    m_angle = (1-PREDATOR_RELAXATION)*atan2(target[1],target[0]) + PREDATOR_RELAXATION*m_angle;

    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

void Agent::obstacleLaw(std::vector<Obstacle>& obstacles, std::vector<size_t>& neighboursObstacles) {

    std::vector<Agent> agents(obstacles.size());
    for (size_t i = 0; i<obstacles.size(); i++) {
        agents[i] = Agent(obstacles[i].get_x(),obstacles[i].get_y());
    }

    vec3 v = this->centerSeparation(agents, neighboursObstacles);
    vec2 separation = normVector({(float)(m_x-v[0]),(float)(m_y-v[1])});

    m_angle = (1-OBSTACLE_RELAXATION)*atan2(separation[1],separation[0]) + OBSTACLE_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::updateAgent(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles) {

    std::vector<std::vector<size_t>> v;
    std::vector<size_t> predators, separation, alignment, cohesion, neighboursObstacles;

    neighboursObstacles = this->obstacle(obstacles);
    v = this->neighbours(agents);

    if (m_predator)
        predators = this->predatorNeighbours(agents);
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
                this->predatorLaw(agents);
        }
    }
    else {
        if (m_obstacle) {
            this->obstacleLaw(obstacles, neighboursObstacles);
        } else {
            if (!predators.empty()) {
                if (!separation.empty())
                    this->biSeparationLaw(agents, separation, predators);
                else
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
    size_t n = agents.size();

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::random_device dev;
    std::mt19937 engine(dev());

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = 2*PI*unif(engine)-PI;

        agent = Agent(randomX,randomY,randomAngle,false,n);
        agent.obstacle(obstacles);
        while (agent.get_obstacle() || agent.overlap(agents)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = 2*PI*unif(engine)-PI;

            agent = Agent(randomX,randomY,randomAngle,false,n);
            agent.obstacle(obstacles);
        }
        agents.push_back(agent);
        n = agents.size();
    }
    for (size_t j = 0; j<DEFAULT_NUM_PREDATORS; ++j) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = 2*PI*unif(engine)-PI;

        agent = Agent(randomX,randomY,randomAngle,true,n);
        agent.obstacle(obstacles);
        while (agent.get_obstacle() || agent.overlap(agents)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = 2*PI*unif(engine)-PI;

            agent = Agent(randomX,randomY,randomAngle,true,n);
            agent.obstacle(obstacles);
        }
        agents.push_back(agent);
        n = agents.size();
    }
    return agents;
}

std::vector<Agent> updateAgents(std::vector<Agent>& agents, std::vector<Obstacle>& obstacles) {
    std::vector<Agent> newAgents;
    size_t n = 0;
    for (size_t i(0); i < agents.size(); i++) {
        agents[i].obstacle(obstacles);
        agents[i].updateAgent(agents, obstacles);
    }
    for (Agent& agent : agents) {
        if ( agent.get_predator() || agent.get_alive() ) {
            agent.get_index() = n;
            newAgents.push_back(agent);
            n++;
        }
    }
    return newAgents;
}
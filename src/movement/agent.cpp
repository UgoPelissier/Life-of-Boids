#include "agent.hpp"

//====================AGENT CLASS============================
Agent::Agent() {
    m_x = 0;
    m_y = 0;
    m_angle = 0;
}

Agent::Agent(double x, double y, double angle, bool predator) {
    m_x = x;
    m_y = y;
    m_angle = angle;
    m_predator = predator;
}

double& Agent::get_x() {
    return m_x;
}

double& Agent::get_y() {
    return m_y;
}

double& Agent::get_angle() {
    return m_angle;
}

bool& Agent::get_predator() {
    return m_predator;
}

double Agent::distance(Agent a) {
    return std::sqrt((m_x - a.get_x()) * (m_x - a.get_x()) + (m_y - a.get_y()) * (m_y - a.get_y()));
}

std::vector<std::vector<size_t>> Agent::neighbours(size_t index, std::vector<Agent> agents) {
    std::vector<size_t> predators, separation, alignment, cohesion;
    for (size_t i(0); i < agents.size(); i++) {
        if (index!=i) {
            if ( !agents[i].get_predator() ) {
                if (this->distance(agents[i]) < SEPARATION) {
                    separation.push_back(i);
                }
                else if (this->distance(agents[i]) > SEPARATION && this->distance(agents[i]) < ALIGNMENT) {
                    alignment.push_back(i);
                }
                else if (this->distance(agents[i]) > ALIGNMENT && this->distance(agents[i]) < COHESION) {
                    cohesion.push_back(i);
                }
            }
            else {
                if (this->distance(agents[i]) < PREDATOR) {
                    predators.push_back(i);
                }
            }
        }
    }
    return {predators, separation, alignment, cohesion};
}

std::vector<size_t> Agent::predatorNeighbours(size_t index, std::vector<Agent> agents) {
    std::vector<size_t> predators;
    for (size_t i(0); i < agents.size(); i++) {
        if ( index!=i ) {
            if ( agents[i].get_predator() ) {
                if ( this->distance(agents[i]) < PREDATOR ) {
                    predators.push_back(i);
                }
            }
        }
    }
    return predators;
}

size_t Agent::closerAgent(size_t index, std::vector<Agent> agents) {
    double d = WIDTH;
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

bool Agent::equal(Agent a) {
    if (m_x==a.get_x() && m_y==a.get_y() && m_angle==a.get_angle()) {
        return true;
    }
    return false;
}

bool Agent::overlap(Agent a) {
    double l = (sqrt(3)*TRIANGLE_SIZE/2)*(WIDTH/2);
    if (this->distance(a)<l) {
        return true;
    }
    return false;
}

bool Agent::overlap(std::vector<Agent> agents) {
    for(Agent& agent : agents) {
        if ( (!this->equal(agent)) && this->overlap(agent) ) {
            return true;
        }
    }
    return false;
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
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::windowUpdate() {
    m_x = modulo(m_x,WIDTH);
    m_y = modulo(m_y, HEIGHT);
}

void Agent::constantUpdate() {
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

vec3 Agent::center(std::vector<Agent> agents, std::vector<size_t> neighbours) {
    size_t n(neighbours.size());
    double x(m_x), y(m_y), angle(m_angle);
    for(size_t& i : neighbours) {
        x += agents[i].get_x();
        y += agents[i].get_y();
        angle += agents[i].get_angle();
    }
    return {(float)(x/(n+1)), (float)(y/(n+1)), (float)(angle/(n+1))};
}

vec3 Agent::centerSeparation(std::vector<Agent> agents, std::vector<size_t> neighbours) {
    size_t n(neighbours.size());
    double x(0), y(0), angle(0);
    for(size_t& i : neighbours) {
        x += agents[i].get_x();
        y += agents[i].get_y();
        angle += agents[i].get_angle();
    }
    return {(float)(x/n), (float)(y/n), (float)(angle/n)};
}

void Agent::cohesionLaw(std::vector<Agent> agents, std::vector<size_t> neighbours) {
    vec3 v = this->center(agents, neighbours);
    m_angle = SMOOTH_COHESION*atan2((v[1]-m_y),(v[0]-m_x)) + (1-SMOOTH_COHESION)*m_angle;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::alignmentLaw(std::vector<Agent> agents, std::vector<size_t> neighbours) {
    vec3 v = this->center(agents, neighbours);
    m_angle = m_angle = SMOOTH_ALIGNMENT*v[2] + (1-SMOOTH_ALIGNMENT)*m_angle ;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::separationLaw(std::vector<Agent> agents, std::vector<size_t> neighbours) {
    vec3 v = this->centerSeparation(agents, neighbours);
    vec2 separation = normVector({(float)(m_x-v[0]),(float)(m_y-v[1])});
    m_x += SPEED * separation[0];
    m_y += SPEED * separation[1];
    m_angle = atan2(separation[1],separation[0]);
}

void Agent::predatorLaw(size_t index, std::vector<Agent> agents) {
    size_t closer = this->closerAgent(index, agents);
    vec2 target = normVector({(float)(agents[closer].get_x() - m_x),(float)(agents[closer].get_y() - m_y)});
    m_x += (SPEED/2) * target[0];
    m_y += (SPEED/2) * target[1];
    m_angle = atan2(target[1],target[0]);
}

void Agent::updateAgent(size_t index, std::vector<Agent> agents) {

    std::vector<std::vector<size_t>> v;
    std::vector<size_t> predators, separation, alignment, cohesion;

    bool tooClose;
    double opp;

    std::tie(tooClose,opp) = this->borders();

    v = this->neighbours(index, agents);

    if (m_predator)
        predators = this->predatorNeighbours(index, agents);
    else
        predators = v[0];

    separation = v[1];
    alignment = v[2];
    cohesion = v[3];

    if (m_predator) {
        if (!predators.empty())
            this->separationLaw(agents, predators);
        else
            this->predatorLaw(index, agents);
    }
    else {
        if (false) {
            this->borderUpdate(opp);
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
std::vector<Agent> initialiaze_agents() {

    std::vector<Agent> agents;
    Agent agent;

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

        agent = Agent(randomX,randomY,randomAngle,false);
        while (agent.borders().first || agent.overlap(agents)) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomAngle = 2*PI*unif(rng)-PI;

            agent = Agent(randomX,randomY,randomAngle,false);
        }
        agents.push_back(agent);
    }
    for (size_t i = 0; i<DEFAULT_NUM_PREDATORS; ++i) {

        randomX = uniX(rng);
        randomY = uniY(rng);
        randomAngle = 2*PI*unif(rng)-PI;

        agent = Agent(randomX,randomY,randomAngle,true);
        while (agent.borders().first || agent.overlap(agents)) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomAngle = 2*PI*unif(rng)-PI;

            agent = Agent(randomX,randomY,randomAngle,true);
        }
        agents.push_back(agent);
    }
    return agents;
}

void updateAgents(std::vector<Agent>& agents) {
    for (size_t i(0); i < agents.size(); i++) {
        agents[i].updateAgent(i, agents);
    }
}
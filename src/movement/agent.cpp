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
    return (std::abs(this->angle(a)) > VIEW_RANGE / 2);
}

std::vector<std::vector<size_t>> Agent::neighbours(agents_t& birds, agents_t& predators) {
    std::vector<size_t> predators_ids, separation_ids, alignment_ids, cohesion_ids;
    Real current_distance;
    for (auto& it : birds) {
        
        size_t i = it.first;
        Agent &agent = it.second;
        
        if (m_index != it.first && this->insideFieldView(agent)) {
            current_distance = this->distance(agent);
            if (current_distance < SEPARATION_RANGE) {
                separation_ids.push_back(i);
            }
            else if (current_distance > SEPARATION_RANGE && current_distance < ALIGNMENT_RANGE) {
                alignment_ids.push_back(i);
            }
            else if (current_distance > ALIGNMENT_RANGE && current_distance < COHESION_RANGE) {
                cohesion_ids.push_back(i);
            }
        }
    }

    for (auto& it : predators) {

        size_t i = it.first;
        Agent& agent = it.second;

        if (m_index != it.first && this->insideFieldView(agent)) {
            current_distance = this->distance(agent);
            if (current_distance < PREDATOR_RANGE) {
                predators_ids.push_back(i);
                if (current_distance < DEAD_RANGE) {
                    m_alive = false;
                    break;
                }
            }
        }
    }

    return {predators_ids, separation_ids, alignment_ids, cohesion_ids};
}

std::vector<size_t> Agent::predatorNeighbours(agents_t& predators) const {
    std::vector<size_t> predators_return_vec;

    for (auto &it : predators) {
        size_t i = it.first;
        Agent& agent = it.second;
        
        if (m_index!= i && agent.get_predator() && this->distance(agent) < PREDATOR_RANGE)
            predators_return_vec.push_back(i);
    }
    return predators_return_vec;
}

size_t Agent::closestAgent(agents_t& birds) const {
    Real d = WIDTH;
    size_t closest_index = SIZE_MAX;

    for (auto &it : birds) {
        
        size_t i = it.first;
        Agent& agent = it.second;

        if (m_index != i) {
            Real current_distance = this->distance(agent);
            if (current_distance < d) {
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

bool Agent::overlap(agents_t& agents) const {
    for(auto &it : agents) {
        Agent& agent = it.second;
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

vec3 Agent::center(agents_t agents, std::vector<size_t>& neighbours) const {
    
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

vec3 Agent::centerSeparation(agents_t& agents, std::vector<size_t> & neighbours) {

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

void Agent::cohesionLaw(agents_t& agents, std::vector<size_t>& neighbours) {

    vec3 v = this->center(agents, neighbours);

    m_angle = (1-COHESION_RELAXATION)*atan2((v[1]-m_y),(v[0]-m_x)) + COHESION_RELAXATION*m_angle;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::alignmentLaw(agents_t& agents, std::vector<size_t>& neighbours) {

    vec3 v = this->center(agents, neighbours);

    m_angle = (1-ALIGNMENT_RELAXATION)*v[2] + ALIGNMENT_RELAXATION*m_angle ;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::separationLaw(agents_t& agents, std::vector<size_t>& neighbours) {

    vec3 v = this->centerSeparation(agents, neighbours);
    vec2 separation = normVector({(float)(m_x-v[0]),(float)(m_y-v[1])});

    m_angle = (1-SEPARATION_RELAXATION)*atan2(separation[1],separation[0]) + SEPARATION_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::biSeparationLaw(agents_t& birds, agents_t& predators, std::vector<size_t>& birdsNeighbours, std::vector<size_t>& predNeighbours) {

    vec3 vBird = this->centerSeparation(birds, birdsNeighbours);
    vec2 separationBird = normVector({(float)(m_x-vBird[0]),(float)(m_y-vBird[1])});

    vec3 vPred = this->centerSeparation(predators, predNeighbours);
    vec2 separationPred = normVector({(float)(m_x-vPred[0]),(float)(m_y-vPred[1])});

    Real angleBird = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;
    Real anglePred = (1-SEPARATION_RELAXATION)*atan2(separationPred[1],separationPred[0]) + SEPARATION_RELAXATION*m_angle;

    m_angle = 0.5*anglePred + 0.5*angleBird;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);

}

void Agent::predatorLaw(agents_t& agents) {

    size_t closest_index = this->closestAgent(agents);
    vec2 target = normVector({(float)(agents[closest_index].get_x() - m_x),(float)(agents[closest_index].get_y() - m_y)});

    m_angle = (1-PREDATOR_RELAXATION)*atan2(target[1],target[0]) + PREDATOR_RELAXATION*m_angle;

    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

void Agent::obstacleLaw(std::vector<Obstacle>& obstacles, std::vector<size_t>& neighboursObstacles) {

    agents_t agents(obstacles.size());
    for (size_t i = 0; i<obstacles.size(); i++) {
        agents[i] = Agent(obstacles[i].get_x(),obstacles[i].get_y());
    }

    vec3 v = this->centerSeparation(agents, neighboursObstacles);
    vec2 separation = normVector({(float)(m_x-v[0]),(float)(m_y-v[1])});

    m_angle = (1-OBSTACLE_RELAXATION)*atan2(separation[1],separation[0]) + OBSTACLE_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::updateBird(agents_t& birds, agents_t& predators, std::vector<Obstacle>& obstacles) {

    /*
    * earlier we had one agent vector now i have created two separate maps for birds and predators
    * The logic is a bit changed where the following size_t vectors for indices will now hold an index
    * with respect to these two different maps
    * So, for example.
    * predators_ids will only have the indices for agents_t predators
    */
    std::vector<std::vector<size_t>> v;
    std::vector<size_t> predators_ids, separation_ids, alignment_ids, cohesion_ids, neighboursObstacles_ids;

    neighboursObstacles_ids = this->obstacle(obstacles);
    v = this->neighbours(birds, predators); // O(n)

    predators_ids = v[0];
    separation_ids = v[1];
    alignment_ids = v[2];
    cohesion_ids = v[3];

    if (m_obstacle) {
        this->obstacleLaw(obstacles, neighboursObstacles_ids); // O(n) 
    } else {
        if (!predators_ids.empty()) {
            if (!separation_ids.empty())
                this->biSeparationLaw(birds, predators, separation_ids, predators_ids); // O(n)
            else
                this->separationLaw(predators, predators_ids); // O(n)
        } else if (!separation_ids.empty()) {
            this->separationLaw(birds, separation_ids); // O(n)
        } else if (!alignment_ids.empty()) {
            this->alignmentLaw(birds, alignment_ids); // O(n)
        } else if (!cohesion_ids.empty()) {
            this->cohesionLaw(birds, cohesion_ids); // O(n)
        } else {
            this->constantUpdate();
        }
    }
    this->windowUpdate();

    // delete the agent from the map if it dies in the current iteration.
    if (!m_alive)
        birds.erase(m_index);
}

void Agent::updatePredator(agents_t& birds, agents_t& predators, std::vector<Obstacle>& obstacles) {

    std::vector<std::vector<size_t>> v;
    std::vector<size_t> predators_ids, separation_ids, alignment_ids, cohesion_ids, neighboursObstacles_ids;

    neighboursObstacles_ids = this->obstacle(obstacles);
    v = this->neighbours(birds, predators); // O(n)

    predators_ids = this->predatorNeighbours(predators); // O(n)
    separation_ids = v[1];
    alignment_ids = v[2];
    cohesion_ids = v[3];

    if (m_obstacle) {
            this->obstacleLaw(obstacles, neighboursObstacles_ids); // O(m)
    }
    else {
        if (!predators_ids.empty())
            this->separationLaw(predators, predators_ids);
        else
            this->predatorLaw(birds);
    }

    this->windowUpdate();
}

//====================RELATED FUNCTIONS============================
std::tuple<agents_t, agents_t> initialiaze_agents(std::vector<Obstacle>& obstacles) {

    agents_t birds, predators;
    Agent bird, predator;

    int randomX;
    int randomY;
    Real randomAngle;
    size_t n = 0;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::random_device dev;
    std::mt19937 engine(dev());

    const size_t surplus = 20;

    //reserve the memory in advance
    birds.reserve(DEFAULT_NUM_AGENTS + surplus);
    predators.reserve(DEFAULT_NUM_PREDATORS + surplus);

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = 2*PI*unif(engine)-PI;
        n = birds.size();
        bird = Agent(randomX,randomY,randomAngle,false,n);
        bird.obstacle(obstacles);
        while (bird.get_obstacle() || bird.overlap(birds) || bird.overlap(predators)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = 2*PI*unif(engine)-PI;

            bird = Agent(randomX,randomY,randomAngle,false,n);
            bird.obstacle(obstacles);
        }
        birds[n] = bird;
    }
    for (size_t j = 0; j<DEFAULT_NUM_PREDATORS; ++j) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = 2*PI*unif(engine)-PI;
        n = predators.size();
        predator = Agent(randomX,randomY,randomAngle,true,n);
        predator.obstacle(obstacles);
        while (predator.get_obstacle() || predator.overlap(birds) || predator.overlap(predators)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = 2*PI*unif(engine)-PI;

            predator = Agent(randomX,randomY,randomAngle,true,n);
            predator.obstacle(obstacles);
        }
        predators[n] = predator;
    }
    return std::make_tuple(birds, predators);
}

void updateAgents(agents_t& birds, agents_t& predators, std::vector<Obstacle>& obstacles) {
    

    for (auto &it : birds) {
        Agent& agent = it.second;
        agent.obstacle(obstacles);
        agent.updateBird(birds, predators, obstacles);
    }

    for (auto& it : predators) {
        Agent& agent = it.second;
        agent.obstacle(obstacles);
        agent.updatePredator(birds, predators, obstacles);
    }
}


/*
* Bottlenecks
* 1. We update agents and then create a new list of agents to be returned for each iteration of the main loop
* 2. We are going through each agent and then update its respective categories like predators, separation, etc.
* Each of these are taking O(n) in the worst case. 
* 3. There are a lot of checks within each for loop.
* 
* Solutions:
* 1. If there is a way to combine all these minor for loops into 1 single big loop
* 2. Managing the category vectors' creation at the initialization phase.
* 3. Maybe using an efficient data structure like queue to push and pop the agents rather than returning a new vector each time.
* 
*/
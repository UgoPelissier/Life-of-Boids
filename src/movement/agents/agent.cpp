#include "agent.h"

Agent::Agent() : Object(), m_angle(0), m_obstacle(false), m_index(0)
{}

Agent::Agent(Real const& x, Real const& y) : Object(x,y), m_angle(0), m_obstacle(false), m_index(0)
{}

Agent::Agent(Real const& x, Real const& y, Real const& angle) : Object(x,y), m_angle(angle), m_obstacle(false), m_index(0)
{}

Agent::Agent(Real const& x, Real const& y, Real const& angle, size_t& index) : Object(x,y), m_angle(angle), m_obstacle(false), m_index(index)
{}

Real Agent::get_angle() const {
    return m_angle;
}

bool Agent::get_obstacle() const {
    return m_obstacle;
}

size_t& Agent::get_index() {
    return m_index;
}

Real Agent::angle(Agent const& a) const {
    vec2 v1 = {(Real)cos(m_angle),(Real)sin(m_angle)};
    vec2 v2 = normVector({a.get_x()-m_x,a.get_y()-m_y});
    Real angle = angleVector(v1,v2);
    return angle;
}

bool Agent::insideFieldView(Agent const& a) const {
    if (std::abs(this->angle(a)) > VIEW_RANGE/2)
        return false;
    return true;
}

bool Agent::operator==(Agent const& a) const {
    return (m_x == a.get_x() && m_y == a.get_y() && m_angle == a.get_angle());
}

bool Agent::overlap(Agent const& a) const {
    if (this->distance(a)<BODY_OVERLAP) {
        return true;
    }
    return false;
}

bool Agent::overlap(std::vector<Agent> const& agents) const {
    for(Agent const& agent : agents) {
        if ( this->operator==(agent) && this->overlap(agent) ) {
            return true;
        }
    }
    return false;
}

std::pair<state,std::vector<Real>> Agent::obstacle(std::vector<Obstacle> const& obstacles) {
    m_obstacle = false;

    state s = constant;
    std::vector<Real> v;

    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;

    for (size_t i(0); i < obstacles.size(); i++) {
        current_distance = this->distance(obstacles[i]);

        if ( current_distance < std::max(obstacles[i].get_height()/2,obstacles[i].get_width()/2) && current_distance<min_distance ) {
            m_obstacle = true;
            s = obst;
            min_distance = current_distance;
            v = {obstacles[i].get_x(),obstacles[i].get_y()};
        }
    }
    return std::make_pair(s,v);
}

void Agent::windowUpdate() {
    m_x = modulo(m_x,WIDTH);
    m_y = modulo(m_y, HEIGHT);
}

void Agent::constantUpdate() {
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

std::pair<state,std::vector<Real>> Agent::neighbour(std::vector<Agent> const& predators, std::vector<Agent> const& birds) const {
    state s = constant;
    std::vector<Real> v;

    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;

    for (size_t i(0); i < predators.size(); i++) {
        current_distance = this->distance(predators[i]);

        if (m_index != i && (current_distance < PREDATOR_RANGE) && (current_distance < min_distance)) {

            if ( this->insideFieldView(predators[i]) ) {
                s = separation;
                min_distance = current_distance;
                v = {predators[i].get_x(),predators[i].get_y()};
            }
        }
    }

    if ( s!=separation ) {
        min_distance = WIDTH+HEIGHT;

        for (size_t i(1); i < birds.size(); i++) {

            if (m_index != i) {
                current_distance = this->distance(birds[i]);

                if (current_distance < min_distance) {

                    if (this->insideFieldView(birds[i])) {
                        min_distance = current_distance;
                        v = {birds[i].get_x(),birds[i].get_y()};
                    }
                }
            }
        }
    }

    return std::make_pair(s,v);
}

Agent Agent::closest(std::vector<Agent> const& birds) const {
    Agent bird;

    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;

    for (size_t i(1); i < birds.size(); i++) {
        if (m_index != i) {
            if (this->insideFieldView(birds[i])) {
                current_distance = this->distance(birds[i]);
                if (current_distance < min_distance) {
                    min_distance = current_distance;
                    bird = birds[i];
                }
            }
        }
    }
    return bird;
}

void Agent::obstacleLaw(std::vector<Real> const&  obstacle) {

    vec2 separation = normVector({(Real)(m_x-obstacle[0]),(Real)(m_y-obstacle[1])});

    m_angle = (1-OBSTACLE_RELAXATION)*atan2(separation[1],separation[0]) + OBSTACLE_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::separationLaw(std::vector<Real> const&  predator) {
    vec2 separationPred = normVector({(Real)(m_x-predator[0]),(Real)(m_y-predator[1])});

    m_angle = (1-SEPARATION_RELAXATION)*atan2(separationPred[1],separationPred[0]) + SEPARATION_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::predatorLaw(std::vector<Real> const& bird) {

    vec2 target = normVector({(Real)(bird[0] - m_x),(Real)(bird[1] - m_y)});

    m_angle = (1-PREDATOR_RELAXATION)*atan2(target[1],target[0]) + PREDATOR_RELAXATION*m_angle;

    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

void Agent::update_predator(std::vector<Obstacle>const& obstacles, std::vector<Agent> const& predators, std::vector<Agent> const& birds) {
    state s = constant;
    std::vector<Real> update;

    // Obstacles
    std::tie(s,update) = this->obstacle(obstacles);
    if (m_obstacle) {
        this->obstacleLaw(update);
    }
    else {
        std::tie(s,update) = this->neighbour(predators, birds);
        if (s==separation)
            this->separationLaw(update);
        else
            this->predatorLaw(update);
    }
    this->windowUpdate();
}

Agent::~Agent() {

}

std::vector<Agent> predators_init(std::vector<Obstacle> const& obstacles) {

    std::vector<Agent> predators;
    Agent predator;

    int randomX;
    int randomY;
    Real randomAngle;
    size_t n = predators.size();

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::random_device dev;
    std::mt19937 engine(dev());

    for (size_t i = 0; i<DEFAULT_NUM_PREDATORS; ++i) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = 2*PI*unif(engine)-PI;

        predator = Agent(randomX,randomY,randomAngle,n);
        predator.obstacle(obstacles);
        while (predator.get_obstacle() || predator.overlap(predators)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = 2*PI*unif(engine)-PI;

            predator = Agent(randomX,randomY,randomAngle,n);
            predator.obstacle(obstacles);
        }
        predators.push_back(predator);
        n = predators.size();
    }
    return predators;
}

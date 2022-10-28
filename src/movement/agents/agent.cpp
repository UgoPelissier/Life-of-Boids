#include "agent.h"
#include "bird.h"
#include "predator.h"

Agent::Agent() : Object(), m_angle(0), m_state(state::constant), m_index(0){}
Agent::Agent(Real const& x, Real const& y) : Object(x,y), m_angle(0), m_state(state::constant), m_index(0){}
Agent::Agent(Real const& x, Real const& y, Real const& angle) : Object(x,y), m_angle(angle), m_state(state::constant), m_index(0){}
Agent::Agent(Real const& x, Real const& y, Real const& angle, size_t& index) : Object(x,y), m_angle(angle), m_state(state::constant), m_index(index){}

Real Agent::get_angle() const {
    return m_angle;
}

state Agent::get_state() const {
    return m_state;
}

size_t& Agent::get_index() {
    return m_index;
}

Real Agent::angle(Agent const& a) const {
    vec2 v1 = {(Real)cos(m_angle), (Real)sin(m_angle)};
    vec2 v2 = calc::normVector({a.get_x()-m_x, a.get_y()-m_y});
    Real angle = calc::angleVector(v1,v2);
    return angle;
}

bool Agent::insideFieldView(Agent const& a) const {
    return (std::abs(this->angle(a)) > VIEW_RANGE / 2);
}

bool Agent::operator==(Agent& a) const {    
    return (m_x == a.get_x() && m_y == a.get_y() && m_angle == a.get_angle());
}

bool Agent::overlap(Agent& a) const {
    return this->distance(a) < BODY_OVERLAP;
}

bool Agent::overlap(birds_t& birds, predators_t& predators) const {

    auto check = [this](auto& it) {
        Agent& a = it.second;
        return this->overlap(a);
    };
    return std::any_of(predators.begin(), predators.end(), check) || 
        std::any_of(birds.begin(), birds.end(), check);
}


std::vector<Real> Agent::obstacle(std::vector<Obstacle> const& obstacles) {
    std::vector<Real> v;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (const auto & obstacle : obstacles) {
        current_distance = this->distance(obstacle);

        if ( current_distance < std::max(obstacle.get_height()/2,obstacle.get_width()/2) && current_distance<min_distance ) {
            m_state = state::near_obstacle;
            min_distance = current_distance;
            v = {obstacle.get_x(),obstacle.get_y()};
        }
    }
    return v;
}

void Agent::windowUpdate() {
    m_x = modulo(m_x,(Real)WIDTH);
    m_y = modulo(m_y, (Real)HEIGHT);
}

void Agent::constantUpdate() {
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::obstacleLaw(std::vector<Real> const&  obstacle) {

    vec2 separation = calc::normVector({(Real)(m_x-obstacle[0]),(Real)(m_y-obstacle[1])});
    m_angle = calc::angle(OBSTACLE_RELAXATION, separation[1], separation[0], m_angle);
    //m_angle = (1-OBSTACLE_RELAXATION)*atan2(separation[1],separation[0]) + OBSTACLE_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Agent::separationLaw(std::vector<Real> const&  predator) {
    vec2 separationPred = calc::normVector({(Real)(m_x-predator[0]),(Real)(m_y-predator[1])});
    m_angle = calc::angle(SEPARATION_RELAXATION, separationPred[1], separationPred[0], m_angle);
    //m_angle = (1-SEPARATION_RELAXATION)*atan2(separationPred[1],separationPred[0]) + SEPARATION_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

Agent::~Agent() = default;

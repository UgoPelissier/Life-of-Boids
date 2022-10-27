#include "agent.h"

Agent::Agent() : Object(), m_angle(0), m_state(constant), m_index(0)
{}

Agent::Agent(Real const& x, Real const& y) : Object(x,y), m_angle(0), m_state(constant), m_index(0)
{}

Agent::Agent(Real const& x, Real const& y, Real const& angle) : Object(x,y), m_angle(angle), m_state(constant), m_index(0)
{}

Agent::Agent(Real const& x, Real const& y, Real const& angle, size_t& index) : Object(x,y), m_angle(angle), m_state(constant), m_index(index)
{}

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

bool Agent::operator==(Agent& a) const {
    
    return (m_x == a.get_x() && m_y == a.get_y() && m_angle == a.get_angle());
}

bool Agent::overlap(Agent& a) const {
    return this->distance(a) < BODY_OVERLAP;
}

bool Agent::overlap(agents_t& agents) const {

    return (std::any_of(agents.begin(),
        agents.end(),
        [this](auto &it) {
            Agent& agent = it.second;
            return this->operator==(agent) && this->overlap(agent);
        }
    ));
}

std::vector<Real> Agent::obstacle(std::vector<Obstacle> const& obstacles) {
    std::vector<Real> v;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (const auto & obstacle : obstacles) {
        current_distance = this->distance(obstacle);

        if ( current_distance < std::max(obstacle.get_height()/2,obstacle.get_width()/2) && current_distance<min_distance ) {
            m_state = obst;
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

std::vector<Real> Agent::neighbour(agents_t& predators, agents_t& birds) {
    std::vector<Real> v;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (auto & pr : predators) {
        Agent& p = pr.second;
        size_t p_index = p.get_index();
        current_distance = this->distance(p);
        
        if (m_index !=  p_index && (current_distance < PREDATOR_RANGE) && (current_distance < min_distance)) {

            if ( this->insideFieldView(p) ) {
                m_state = separation;
                min_distance = current_distance;
                v = {p.get_x(), p.get_y()};
            }
        }
    }

    if ( m_state!=separation ) {
        min_distance = (Real)(WIDTH+HEIGHT);

        for (auto const& br : birds) {
            
            auto &b = br.second;
            current_distance = this->distance(b);

            if (current_distance < min_distance) {

                if (this->insideFieldView(b)) {
                    m_state = predator;
                    min_distance = current_distance;
                    v = {b.get_x(), b.get_y()};
                }
            }
        }
    }

    return v;
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

int Agent::update_predator(std::vector<Obstacle>const& obstacles, agents_t& predators, agents_t& birds) {
    m_state = constant;
    std::vector<Real> update;

    // Obstacles
    update = this->obstacle(obstacles);

    if (m_state==obst) {
        this->obstacleLaw(update);
        this->windowUpdate();
        return 0;
    }
    // Neighbours and preys
    update = this->neighbour(predators, birds);

    switch ( m_state )
    {
        case predator:
            this->predatorLaw(update);
            this->windowUpdate();
            return 0;
        case separation:
            this->separationLaw(update);
            this->windowUpdate();
            return 0;
        // for default or constant, do a constant update        
        default:
        case constant:
            this->constantUpdate();
            this->windowUpdate();
            return 0;
    }
}

Agent::~Agent() = default;

agents_t predators_init(std::vector<Obstacle> const& obstacles) {

    agents_t predators;
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
        randomAngle = (Real)(2*PI*unif(engine)-PI);

        predator = Agent((Real)randomX,(Real)randomY,randomAngle,n);
        predator.obstacle(obstacles);
        while (predator.get_state()==obst || predator.overlap(predators)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = (Real)2*PI*unif(engine)-PI;

            predator = Agent((Real)randomX,(Real)randomY,randomAngle,n);
            predator.obstacle(obstacles);
        }
        predators[n] = predator;
        n = predators.size();
    }

    return predators;
}

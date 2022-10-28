#include "bird.h"
#include "predator.h"

Predator::Predator() : Agent() {}
Predator::Predator(Real const& x, Real const& y) : Agent(x, y) {}
Predator::Predator(Real const& x, Real const& y, Real const& angle) : Agent(x, y, angle) {}
Predator::Predator(Real const& x, Real const& y, Real const& angle, size_t& index) : Agent(x, y, angle, index) {}

std::vector<Real> Predator::neighbour(birds_t& birds, predators_t& predators) {
    std::vector<Real> v;

    Real current_distance;
    Real min_distance = (Real)(WIDTH + HEIGHT);

    for (auto& it : predators) {
    
        Predator& p = it.second;
        size_t p_index = p.get_index();
        current_distance = this->distance(p);

        if (m_index != p_index && (current_distance < PREDATOR_RANGE) && (current_distance < min_distance)) {

            if (this->insideFieldView(p)) {
                m_state = state::separation;
                min_distance = current_distance;
                v = { p.get_x(), p.get_y() };
            }
        }
    }

    if (m_state != state::separation) {

        min_distance = (Real)(WIDTH + HEIGHT);
        for (auto& it : birds) {

            Bird& b = it.second;
            current_distance = this->distance(b);

            if (current_distance < min_distance) {

                if (this->insideFieldView(b)) {
                    m_state = state::near_predator;
                    min_distance = current_distance;
                    v = { b.get_x(), b.get_y() };
                }
            }
        }
    }

    return v;
}


void Predator::predatorLaw(std::vector<Real> const& bird) {

    vec2 target = calc::normVector({ (Real)(bird[0] - m_x),(Real)(bird[1] - m_y) });
    m_angle = calc::angle(PREDATOR_RELAXATION, target[1], target[0], m_angle);
    //m_angle = (1 - PREDATOR_RELAXATION) * atan2(target[1], target[0]) + PREDATOR_RELAXATION * m_angle;

    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

void Predator::update(std::vector<Obstacle>const& obstacles, predators_t& predators, birds_t& birds) {
    m_state = state::constant;
    std::vector<Real> update;

    // Obstacles
    update = this->obstacle(obstacles);

    if (m_state == state::near_obstacle) {
        this->obstacleLaw(update);
        this->windowUpdate();
    }
    // Neighbours and preys
    update = this->neighbour(birds, predators);

    switch (m_state) {

        case state::near_predator:
            this->predatorLaw(update);
            break;
        case state::separation:
            this->separationLaw(update);
            break;
        // for default or constant, do a constant update        
        default:
        case state::constant:
            this->constantUpdate();
            break;
    }

    // update the window
    this->windowUpdate();
}

Predator::~Predator() = default;

predators_t Predator::init(std::vector<Obstacle> const& obstacles) {

    predators_t predators;
    birds_t birds;
    Predator predator;

    int randomX;
    int randomY;
    Real randomAngle;
    size_t n = predators.size();

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::random_device dev;
    std::mt19937 engine(dev());

    for (size_t i = 0; i < DEFAULT_NUM_PREDATORS; ++i) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = (Real)(2 * PI * unif(engine) - PI);

        predator = Predator((Real)randomX, (Real)randomY, randomAngle, n);
        predator.obstacle(obstacles);
        while (predator.get_state() == state::near_obstacle || predator.overlap(birds, predators)) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = (Real)2 * PI * unif(engine) - PI;

            predator = Predator((Real)randomX, (Real)randomY, randomAngle, n);
            predator.obstacle(obstacles);
        }
        predators[n] = predator;
        n = predators.size();
    }

    return predators;
}

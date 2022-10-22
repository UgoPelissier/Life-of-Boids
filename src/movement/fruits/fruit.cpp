#include "fruit.h"

Fruit::Fruit() : Object(), m_size(0), m_obstacle(false), m_alive(true)
{}

Fruit::Fruit(Real const& x, Real const& y) : Object(x,y), m_size(0), m_obstacle(false), m_alive(true)
{}

Fruit::Fruit(Real const& x, Real const& y, Real const& size, bool const& alive) : Object(x,y), m_size(size), m_obstacle(false), m_alive(alive)
{}

Real Fruit::get_size() const {
    return m_size;
}

bool Fruit::get_obstacle() const {
    return m_obstacle;
}

bool& Fruit::get_alive() {
    return m_alive;
}

bool Fruit::borders() const {
    if (m_x < (Real)CLOSE) {
        return true;
    }
    else if (( (Real)WIDTH- (Real)CLOSE < m_x) && (m_x < (Real)WIDTH)) {
        return true;
    }
    else if (m_y < CLOSE) {
        return true;
    }
    else if (( (Real)HEIGHT- (Real)CLOSE ) < m_y && (m_y < (Real)HEIGHT) ) {
        return true;
    }
    return false;
}

void Fruit::obstacle(std::vector<Obstacle> const& obstacles) {
    m_obstacle = false;
    for (const auto & obstacle : obstacles) {
        if ( this->distance(obstacle) < std::max(obstacle.get_height()/2,obstacle.get_width()/2) ) {
            m_obstacle = true;
        }
    }
}

Fruit::~Fruit() = default;
#include "fruit.h"

Fruit::Fruit() : Object(), m_size(0), m_obstacle(false), m_alive(true)
{}

Fruit::Fruit(Real const& x, Real const& y) : Object(x,y), m_size(0), m_obstacle(false), m_alive(true)
{}

Fruit::Fruit(Real const& x, Real const& y, Real const& size, bool const& alive) : Object(x,y), m_size(size), m_alive(alive)
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
    if (m_x < CLOSE) {
        return true;
    }
    else if ((WIDTH - CLOSE < m_x) && (m_x < WIDTH)) {
        return true;
    }
    else if (m_y < CLOSE) {
        return true;
    }
    else if ((HEIGHT - CLOSE < m_y) && (m_y < HEIGHT)) {
        return true;
    }
    return false;
}

void Fruit::obstacle(std::vector<Obstacle> const& obstacles) {
    m_obstacle = false;
    for (size_t i(0); i < obstacles.size(); i++) {
        if ( this->distance(obstacles[i]) < std::max(obstacles[i].get_height()/2,obstacles[i].get_width()/2) ) {
            m_obstacle = true;
        }
    }
}

Fruit::~Fruit() {

}
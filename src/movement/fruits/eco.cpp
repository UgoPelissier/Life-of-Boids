#include "eco.h"

Eco::Eco() : Object() {}
Eco::Eco(Real const& x, Real const& y) : Object(x, y), m_obstacle(false) {}

bool Eco::get_obstacle() const {
    return m_obstacle;
}

void Eco::closestObstacle(std::vector<Obstacle> const& obstacles) {

    m_obstacle = false;
    for (Obstacle const& obstacle : obstacles) {
        if (this->distance(obstacle) < std::max(obstacle.get_height() * HALF, obstacle.get_width() * HALF)) {
            m_obstacle = true;
            break;
        }
    }
}
#include "eco.h"

Eco::Eco() : Object(), m_obstacle(false) {}
Eco::Eco(Real const& x, Real const& y) : Object(x, y), m_obstacle(false) {}

bool Eco::get_obstacle() const {
    return m_obstacle;
}

bool Eco::nearBorder() const {

    return (m_x < (Real)CLOSE) || (((Real)WIDTH - (Real)CLOSE < m_x) && (m_x < (Real)WIDTH)) ||
           (m_y < CLOSE) || (((Real)HEIGHT - (Real)CLOSE) < m_y && (m_y < (Real)HEIGHT));
}

void Eco::closestObstacle(std::vector<Obstacle> const& obstacles) {

    auto check = [this](Obstacle const& obstacle) {
        return (this->distance(obstacle) < std::max(obstacle.get_height() * HALF, obstacle.get_width() * HALF));
    };
    
    m_obstacle = std::any_of(obstacles.begin(), obstacles.end(), check);
}

Eco::~Eco() = default;
#include "obstacle.h"

Obstacle::Obstacle() {
    m_x = 0;
    m_y = 0;
    m_height = 0;
    m_width = 0;
}

Obstacle::Obstacle(double const& x, double const& y, double const& height, double const& width) {
    m_x = x;
    m_y = y;
    m_height = height;
    m_width = width;
}

double& Obstacle::get_x() {
    return m_x;
}

double& Obstacle::get_y() {
    return m_y;
}

double& Obstacle::get_height() {
    return m_height;
}

double& Obstacle::get_width() {
    return m_width;
}

bool Obstacle::equal(Obstacle& a) {
    if (m_x == a.get_x() && m_y == a.get_y() && m_height == a.get_height() && m_width == a.get_width()) {
        return true;
    }
    return false;
}

Obstacle randomObstacle() {
    Obstacle obstacle;

    int randomX;
    int randomY;
    int randomHeight;
    int randomWidth;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::uniform_int_distribution uniSize(0, MAX_OBSTACLE_SIZE);
    std::mt19937_64 rng;

    randomX = uniX(rng);
    randomY = uniY(rng);
    randomHeight = uniSize(rng);
    randomWidth = uniSize(rng);

    obstacle = Obstacle(randomX, randomY, randomHeight, randomWidth);

    return obstacle;
}


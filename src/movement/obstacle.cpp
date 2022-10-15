#include "obstacle.h"

Obstacle::Obstacle() {
    m_x = 0;
    m_y = 0;
    m_height = 0;
    m_width = 0;
}

Obstacle::Obstacle(Real const& x, Real const& y, Real const& height, Real const& width) {
    m_x = x;
    m_y = y;
    m_height = height;
    m_width = width;
}

Real& Obstacle::get_x() {
    return m_x;
}

Real& Obstacle::get_y() {
    return m_y;
}

Real& Obstacle::get_height() {
    return m_height;
}

Real& Obstacle::get_width() {
    return m_width;
}

bool Obstacle::borders(){
    if ( m_x<CLOSE ) {
        return true;
    }
    else if ( (WIDTH-CLOSE<m_x) && (m_x<WIDTH) ) {
        return true;
    }
    else if ( m_y<CLOSE ) {
        return true;
    }
    else if ( (HEIGHT-CLOSE<m_y) && (m_y<HEIGHT) ) {
        return true;
    }
    return false;
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

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
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

std::vector<Obstacle> initObstacles() {
    std::vector<Obstacle> obstacles;
    Obstacle newObstacle;

    int randomX;
    int randomY;
    int randomHeight;
    int randomWidth;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::uniform_int_distribution uniSize(0, MAX_OBSTACLE_SIZE);
    std::mt19937_64 rng;

    while (obstacles.size()<DEFAULT_NUM_OBSTACLES) {
        randomX = uniX(rng);
        randomY = uniY(rng);
        randomHeight = uniSize(rng);
        randomWidth = uniSize(rng);
        newObstacle = Obstacle(randomX, randomY, randomHeight, randomWidth);

        while ( newObstacle.borders() ) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomHeight = uniSize(rng);
            randomWidth = uniSize(rng);
            newObstacle = Obstacle(randomX, randomY, randomHeight, randomWidth);

            obstacles.push_back(newObstacle);
        }
    }
    return obstacles;
}


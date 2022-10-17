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

bool Obstacle::borders() const{
    if (m_x < CLOSE)
        return true;
    else if ((WIDTH - CLOSE < m_x) && (m_x < WIDTH)) {
        return true;
    } else if (m_y < CLOSE) {
        return true;
    } else if ((HEIGHT - CLOSE < m_y) && (m_y < HEIGHT)) {
        return true;
    }
    return false;
}

std::vector<Obstacle> initObstacles() {
    std::vector<Obstacle> obstacles;
    Obstacle newObstacle;

    int randomX;
    int randomY;
    int randomHeight;
    int randomWidth;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(10, WIDTH-10);
    std::uniform_int_distribution uniY(10, HEIGHT-10);
    std::uniform_int_distribution uniSize(1, MAX_OBSTACLE_SIZE);
    std::random_device dev;
    std::mt19937 engine(dev());

    while (obstacles.size()<DEFAULT_NUM_OBSTACLES) {
        randomX = uniX(engine);
        randomY = uniY(engine);
        randomHeight = uniSize(engine);
        randomWidth = uniSize(engine);
        newObstacle = Obstacle(randomX, randomY, randomHeight, randomWidth);

        while ( newObstacle.borders() ) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomHeight = uniSize(engine);
            randomWidth = uniSize(engine);
            newObstacle = Obstacle(randomX, randomY, randomHeight, randomWidth);

        }
        obstacles.push_back(newObstacle);
    }
    return obstacles;
}


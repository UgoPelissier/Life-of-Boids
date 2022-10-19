#include "obstacle.h"

Obstacle::Obstacle() : Object(), m_height(0), m_width(0)
{}

Obstacle::Obstacle(Real const& x, Real const& y, Real const& height, Real const& width) : Object(x,y), m_height(height), m_width(width)
{}

Real Obstacle::get_height() const {
    return m_height;
}

Real Obstacle::get_width() const {
    return m_width;
}

bool Obstacle::borders() const {
    if ( m_x < m_width/2 )
        return true;
    else if ((WIDTH - m_width/2 < m_x) && (m_x < WIDTH)) {
        return true;
    } else if (m_y < m_height/2) {
        return true;
    } else if ((HEIGHT - m_height/2 < m_y) && (m_y < HEIGHT)) {
        return true;
    }
    return false;
}

bool Obstacle::overlap(std::vector<Obstacle>  const& obstacles) const {
    for (Obstacle  const& obs : obstacles) {
        if ( ( std::abs(obs.get_x()-m_x) <= (obs.get_width()/2 + m_width/2 + ALIGNMENT_RANGE) ) && ( std::abs(obs.get_y()-m_y) <= (obs.get_height()/2 + m_height/2 + ALIGNMENT_RANGE) ) ) {
            return true;
        }
    }
    return false;
}

Obstacle::~Obstacle() {

}

std::vector<Obstacle> obstacles_init() {
    std::vector<Obstacle> obstacles;
    Obstacle newObstacle;

    int randomX;
    int randomY;
    int randomHeight;
    int randomWidth;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::uniform_int_distribution uniSize(10, MAX_OBSTACLE_SIZE);
    std::random_device dev;
    std::mt19937 engine(dev());

    while (obstacles.size()<DEFAULT_NUM_OBSTACLES) {
        randomX = uniX(engine);
        randomY = uniY(engine);
        randomHeight = uniSize(engine);
        randomWidth = uniSize(engine);
        newObstacle = Obstacle(randomX, randomY, randomHeight, randomWidth);

        while ( newObstacle.overlap(obstacles) || newObstacle.borders() ) {
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
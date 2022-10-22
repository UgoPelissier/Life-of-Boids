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
    else if (( (Real)WIDTH - m_width/2 < m_x) && (m_x < (Real)WIDTH)) {
        return true;
    } else if (m_y < m_height/2) {
        return true;
    } else if (( (Real)HEIGHT - m_height/2 < m_y) && (m_y < (Real)HEIGHT)) {
        return true;
    }
    return false;
}

bool Obstacle::overlap(std::vector<Obstacle>  const& obstacles) const {

    if (std::any_of(obstacles.begin(),
                    obstacles.end(),
                    [this](Obstacle const& obs){ return ( (( std::abs(obs.get_x()-m_x) <= (obs.get_width()/2 + m_width/2 + ALIGNMENT_RANGE) )
                                                           && ( std::abs(obs.get_y()-m_y) <= (obs.get_height()/2 + m_height/2 + ALIGNMENT_RANGE) )) ) ;}
                                                           ) ) {
        return true;
    }
    return false;
}

Obstacle::~Obstacle() = default;

std::vector<Obstacle> obstacles_init() {
    std::vector<Obstacle> obstacles;
    Obstacle newObstacle;

    Real randomX;
    Real randomY;
    Real randomHeight;
    Real randomWidth;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_real_distribution<Real> uniX(0, (Real)WIDTH);
    std::uniform_real_distribution<Real> uniY(0, (Real)HEIGHT);
    std::uniform_real_distribution<Real> uniSize(10, (Real)MAX_OBSTACLE_SIZE);
    std::random_device dev;
    std::mt19937 engine(dev());

    while (obstacles.size()<DEFAULT_NUM_OBSTACLES) {
        randomX = uniX(engine);
        randomY = uniY(engine);
        randomHeight = uniSize(engine);
        randomWidth = uniSize(engine);
        newObstacle = Obstacle((Real)randomX, (Real)randomY, (Real)randomHeight, (Real)randomWidth);

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
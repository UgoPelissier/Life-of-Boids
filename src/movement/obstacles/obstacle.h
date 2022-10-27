#pragma once
#include "object.h"

class Obstacle : public Object {
protected :
    Real m_height;
    Real m_width;

public:
    Obstacle();
    Obstacle(Real const& x, Real const& y, Real const& height, Real const& width);

    Real get_height() const;
    Real get_width() const;

    bool borders() const;
    bool overlap(std::vector<Obstacle>  const& obstacles) const;
    static std::vector<Obstacle> init();
    ~Obstacle();
};


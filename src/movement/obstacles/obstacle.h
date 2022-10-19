#pragma once
#include "../object.h"

class Obstacle : public Object {
protected :
    Real m_height;
    Real m_width;

public:
    Obstacle();
    Obstacle(Real const& x, Real const& y, Real const& height, Real const& width);

    virtual Real get_height() const;
    virtual Real get_width() const;

    virtual bool borders() const;
    virtual bool overlap(std::vector<Obstacle>  const& obstacles) const;

    virtual ~Obstacle();
};

std::vector<Obstacle> obstacles_init();
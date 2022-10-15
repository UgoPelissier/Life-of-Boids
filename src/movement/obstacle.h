#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include "../config/config.h"

class Obstacle {
private :
    Real m_x;
    Real m_y;
    Real m_height;
    Real m_width;

public:
    Obstacle();
    Obstacle(Real const& x, Real const& y, Real const& height, Real const& width);

    Real& get_x();
    Real& get_y();
    Real& get_height();
    Real& get_width();

    bool borders();

    bool equal(Obstacle& a);
};

Obstacle randomObstacle();

std::vector<Obstacle> initObstacles();
#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include "../constants/constant.h"

class Obstacle {
private :
    double m_x;
    double m_y;
    double m_height;
    double m_width;

public:
    Obstacle();
    Obstacle(double const& x, double const& y, double const& height, double const& width);

    double& get_x();
    double& get_y();
    double& get_height();
    double& get_width();

    bool equal(Obstacle& a);
};

Obstacle randomObstacle();
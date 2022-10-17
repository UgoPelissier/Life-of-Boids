#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <ctime>
#include "../config/config.h"
#include "obstacle.h"

class Fruit {
private:
    Real m_x;
    Real m_y;
    Real m_size;
    bool m_alive;

public:
    Fruit();
    Fruit(Real const& x, Real const& y, Real const& size, bool const& alive);

    Real& get_x();
    Real& get_y();
    Real& get_size();
    bool& get_alive();

};

class FruitTree {
private:
    Real m_x;
    Real m_y;
    Real m_height;
    Real m_width;
    double m_time; //time for new fruit batch

public:
    FruitTree();
    FruitTree(Real const& x, Real const& y, Real const& height, Real const& width, double const& m_time);

    Real& get_x();
    Real& get_y();
    Real& get_height();
    Real& get_width();
    double get_time();

    bool borders();

    std::vector <Fruit> DropFruit(std::vector < Fruit >& fruits, std::vector<Obstacle> obstacles);
};

FruitTree randomFruitTree();

std::vector<FruitTree> initFruitTree(std::vector<Obstacle> obstacles);

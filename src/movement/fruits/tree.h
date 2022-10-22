#pragma once
#include "object.h"
#include "obstacle.h"
#include "fruit.h"

class Tree : public Object {
    Real m_height;
    Real m_width;
    bool m_obstacle;
    double m_time; //Time for new fruit batch

public:
    Tree();
    Tree(Real const& x, Real const& y, Real const& height, Real const& width, double const& time);

    virtual Real get_height() const;
    virtual Real get_width() const;
    virtual bool get_obstacle() const;

    virtual bool borders() const;
    virtual void obstacle(std::vector<Obstacle> const& obstacles);

    virtual std::vector<Fruit> DropFruit(std::vector<Fruit>& fruits, std::vector<Obstacle> const& obstacles);

    virtual ~Tree();
};

std::vector<Tree> trees_init(std::vector<Obstacle> const& obstacles);
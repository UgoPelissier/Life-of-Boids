#pragma once
#include "eco.h"
#include "fruit.h"

class Tree : public Eco {
protected:
    Real m_height;
    Real m_width;
    double m_time; //Time for new fruit batch
public:
    Tree();
    Tree(Real const& x, Real const& y, Real const& height, Real const& width, double const& time);

    Real get_height() const;
    Real get_width() const;

    void DropFruitAndAppend(std::vector<Fruit>& fruits, std::vector<Obstacle> const& obstacles);

    static std::vector<Tree> init(std::vector<Obstacle> const& obstacles);
    
    ~Tree();
};

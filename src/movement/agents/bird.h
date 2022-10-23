#pragma once
#include "object.h"
#include "agent.h"
#include "fruit.h"

class Bird : public Agent {
protected:
    bool m_alive;

public:
    Bird();
    Bird(Real const& x, Real const& y);
    Bird(Real const& x, Real const& y, Real const& angle);
    Bird(Real const& x, Real const& y, Real const& angle, size_t& index);

    virtual bool get_alive() const;

    virtual std::vector<Real> neighbours(std::vector<Bird> const& birds);
    virtual std::vector<Real> pred(std::vector<Agent> const& predators);
    virtual std::vector<Real> fruits(std::vector<Fruit>& fruits, std::vector<Bird>& birds);

    virtual void cohesionLaw(std::vector<Real> const& group);
    virtual void alignmentLaw(std::vector<Real> const& group);

    virtual void biSeparationLaw(std::vector<Real> const& bird, std::vector<Real> const&  predator);

    virtual void fruitLaw(std::vector<Real> const& f);
    virtual void biFruitLaw(std::vector<Real> const& f, std::vector<Real> const& bird);

    virtual int update(std::vector<Obstacle>const& obstacles, std::vector<Agent> const& predators, std::vector<Bird>& birds, std::vector<Fruit>& fruits);

    virtual ~Bird();
};

std::vector<Agent> birds2agents(std::vector<Bird> const& birds);

std::vector<Bird> birds_init(std::vector<Obstacle> const& obstacles, std::vector<Agent> const& predators);


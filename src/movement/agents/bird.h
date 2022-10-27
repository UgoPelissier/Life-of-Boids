#pragma once
#include "object.h"
#include "agent.h"
#include "fruit.h"

class Bird;
using birds_t = std::unordered_map<size_t, Bird>;

class Bird : public Agent {
protected:
    bool m_alive;

public:
    Bird();
    Bird(Real const& x, Real const& y);
    Bird(Real const& x, Real const& y, Real const& angle);
    Bird(Real const& x, Real const& y, Real const& angle, size_t& index);

    virtual bool get_alive() const;

    virtual std::vector<Real> neighbours(birds_t& birds);
    virtual std::vector<Real> pred(agents_t& predators);
    virtual std::vector<Real> fruits(std::vector<Fruit>& fruits, birds_t& birds);

    virtual void cohesionLaw(std::vector<Real> const& group);
    virtual void alignmentLaw(std::vector<Real> const& group);

    virtual void biSeparationLaw(std::vector<Real> const& bird, std::vector<Real> const&  predator);

    virtual void fruitLaw(std::vector<Real> const& f);
    virtual void biFruitLaw(std::vector<Real> const& f, std::vector<Real> const& bird);

    virtual bool update(std::vector<Obstacle>const& obstacles, agents_t& predators, birds_t& birds, std::vector<Fruit>& fruits);

    virtual ~Bird();
};

agents_t birds2agents(birds_t& birds);

birds_t birds_init(std::vector<Obstacle> const& obstacles, agents_t& predators);


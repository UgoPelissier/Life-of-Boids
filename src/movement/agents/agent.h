#pragma once
#include "object.h"
#include "obstacle.h"

class Agent;
enum state {obst, predator, predatorANDseparation, fruit, fruitANDseparation, separation, alignment, cohesion, constant};
using agents_t = std::unordered_map<size_t, Agent>;

class Agent : public Object {
protected:
    Real m_angle;
    state m_state;
    size_t m_index;

public:
    Agent();
    Agent(Real const& x, Real const& y);
    Agent(Real const& x, Real const& y, Real const& angle);
    Agent(Real const& x, Real const& y, Real const& angle, size_t& index);

    virtual Real get_angle() const;
    virtual state get_state() const;
    virtual size_t get_index();

    virtual Real angle (Agent const& a) const;
    virtual bool insideFieldView(Agent const& a) const;

    virtual bool operator==(Agent const& a) const;
    virtual bool overlap(Agent const& a) const;
    virtual bool overlap(agents_t const& agents) const;

    virtual std::vector<Real> obstacle(std::vector<Obstacle> const& obstacles);

    virtual void windowUpdate();
    virtual void constantUpdate();

    virtual std::vector<Real> neighbour(agents_t & predators, agents_t & birds);

    virtual void obstacleLaw(std::vector<Real> const&  obstacle);
    virtual void separationLaw(std::vector<Real> const&  predator);
    virtual void predatorLaw(std::vector<Real> const& birds);

    virtual int update_predator(std::vector<Obstacle>const& obstacles, agents_t const& predators, agents_t const& birds);

    virtual ~Agent();
};

agents_t predators_init(std::vector<Obstacle> const& obstacles);
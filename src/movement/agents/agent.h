#pragma once
#include "../object.h"
#include "../obstacles/obstacle.h"

class Agent : public Object {
protected:
    Real m_angle;
    bool m_obstacle;
    size_t m_index;

public:
    Agent();
    Agent(Real const& x, Real const& y);
    Agent(Real const& x, Real const& y, Real const& angle);
    Agent(Real const& x, Real const& y, Real const& angle, size_t& index);

    virtual Real get_angle() const;
    virtual bool get_obstacle() const;
    virtual size_t& get_index();

    virtual Real angle (Agent const& a) const;
    virtual bool insideFieldView(Agent const& a) const;

    virtual bool operator==(Agent const& a) const;
    virtual bool overlap(Agent const& a) const;
    virtual bool overlap(std::vector<Agent> const& agents) const;

    virtual std::vector<size_t> obstacle(std::vector<Obstacle> const& obstacles);

    virtual void windowUpdate();
    virtual void constantUpdate();

    virtual std::vector<size_t> neighbour(std::vector<Agent> const& predators) const;
    virtual Agent closest(std::vector<Agent> const& birds) const;

    virtual vec3 center(std::vector<Agent> const& agents, std::vector<size_t> const& neighbours) const;

    virtual void obstacleLaw(std::vector<Obstacle> const& obstacles, std::vector<size_t> const& neighboursObstacle);
    virtual void separationLaw(Agent const&  predator);
    virtual void predatorLaw(std::vector<Agent> const& birds);

    virtual void update_predator(std::vector<Obstacle>const& obstacles, std::vector<Agent> const& predators, std::vector<Agent> const& birds);

    virtual ~Agent();
};

std::vector<Agent> predators_init(std::vector<Obstacle> const& obstacles);
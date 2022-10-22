#pragma  once
#include "object.h"
#include "obstacle.h"

class Fruit : public Object {
protected:
    Real m_size;
    bool m_obstacle;
    bool m_alive;

public:
    Fruit();
    Fruit(Real const& x, Real const& y);
    Fruit(Real const& x, Real const& y, Real const& size, bool const& alive);

    virtual Real get_size() const;
    virtual bool get_obstacle() const;
    virtual bool& get_alive();

    virtual bool borders() const;
    virtual void obstacle(std::vector<Obstacle> const& obstacles);

    virtual ~Fruit();
};

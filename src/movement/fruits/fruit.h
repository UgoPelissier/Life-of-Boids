#pragma  once
#include "eco.h"

class Fruit : public Eco {

protected:
    Real m_size;
    bool m_alive;

public:
    Fruit();
    Fruit(Real const& x, Real const& y);
    Fruit(Real const& x, Real const& y, Real const& size, bool const& alive);

    Real get_size() const;
    bool& get_alive();

    ~Fruit();
};

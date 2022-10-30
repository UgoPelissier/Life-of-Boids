#include "fruit.h"

Fruit::Fruit() : Eco(), m_size(0), m_alive(true) {}
Fruit::Fruit(Real const& x, Real const& y) : Eco(x,y), m_size(0), m_alive(true) {}
Fruit::Fruit(Real const& x, Real const& y, Real const& size, bool const& alive) : Eco(x,y), m_size(size),m_alive(alive) {}

Real Fruit::get_size() const {
    return m_size;
}

bool& Fruit::get_alive() {
    return m_alive;
}

Fruit::~Fruit() = default;
#include "object.h"

Object::Object() : m_x(0), m_y(0)
{}
Object::Object(Real const& x, Real const& y) : m_x(x), m_y(y)
{}

Real Object::get_x() const{
    return m_x;
}

Real Object::get_y() const{
    return m_y;
}

Real Object::distance(Object const& object) const {
    Real diff_x = (m_x - object.get_x());
    Real diff_y = (m_y - object.get_y());

    return std::sqrt((diff_x * diff_x) + (diff_y * diff_y));
}

Object::~Object() = default;

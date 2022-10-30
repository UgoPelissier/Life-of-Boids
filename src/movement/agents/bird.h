#pragma once

#include "agent.h"
#include "predator.h"
#include "bird.h"
#include "fruits/fruit.h"
#include <mutex>

/*
#if DEFAULT_NUM_BIRDS > 1200
const size_t RESERVE_COUNT = (size_t) DEFAULT_NUM_BIRDS * .05;
#else
const size_t RESERVE_COUNT = 10;
#endif
*/

const size_t RESERVE_COUNT = DEFAULT_NUM_BIRDS;

class Bird : public Agent {
private:
    static std::mutex bird_mtx;
protected:
    bool m_alive;
public:
    // std::unordered_map<size_t, bool> m_ignore_ids;
    
    Bird();
    Bird(Real const& x, Real const& y);
    Bird(Real const& x, Real const& y, Real const& angle);
    Bird(Real const& x, Real const& y, Real const& angle, size_t& index);

    bool get_alive() const;

    std::vector<Real> neighbours(birds_t& birds);
    std::vector<Real> Bird::neighbours_par(birds_t& birds);
    std::vector<Real> closestPredator(predators_t& predators);
    std::vector<Real> closestFruit(std::vector<Fruit>& fruits);

    void cohesionLaw(std::vector<Real> const& group);
    void alignmentLaw(std::vector<Real> const& group);
    void biSeparationLaw(std::vector<Real> const& bird, std::vector<Real> const&  predator);
    void fruitLaw(std::vector<Real> const& f);
    void biFruitLaw(std::vector<Real> const& f, std::vector<Real> const& bird);

    bool update(std::vector<Obstacle>const& obstacles, predators_t& predators, birds_t& birds, std::vector<Fruit>& fruits);

    static birds_t init(std::vector<Obstacle> const& obstacles, predators_t& predators);

    /* bool canIgnore(size_t i);
     void resetIgnore();
     static void clearIgnore(birds_t& birds);
     */
    ~Bird();
};

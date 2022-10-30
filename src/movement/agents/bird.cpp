#include <algorithm>
#include <execution>
#include "bird.h"
#include "predator.h"

std::mutex Bird::bird_mtx;

Bird::Bird() : Agent(), m_alive(true) {
    // m_ignore_ids.reserve(RESERVE_COUNT);
}
Bird::Bird(Real const& x, Real const& y) : Agent(x,y), m_alive(true) {
    // m_ignore_ids.reserve(RESERVE_COUNT);
}
Bird::Bird(Real const& x, Real const& y, Real const& angle) : Agent(x,y,angle), m_alive(true) {
    // m_ignore_ids.reserve(RESERVE_COUNT);
}
Bird::Bird(Real const& x, Real const& y, Real const& angle, size_t& index) : Agent(x,y,angle,index), m_alive(true) {
    // m_ignore_ids.reserve(RESERVE_COUNT);
}

bool Bird::get_alive() const {
    return m_alive;
}
/*
bool Bird::canIgnore(size_t i) {

    if (m_ignore_ids.find(i) == m_ignore_ids.end())
        return false;
    return m_ignore_ids[i];
}

void Bird::resetIgnore() {
    //m_ignore_ids.clear();
    std::for_each(m_ignore_ids.begin(), m_ignore_ids.end(), [](auto &it) {it.second = false;});
}
*/
std::vector<Real> Bird::neighbours(birds_t& birds) {
    std::vector<Real> v;
    Real xSeparation(0), ySeparation(0);

    Real angleAlignment(0);
    int nAlignment(0);

    Real xCohesion(0), yCohesion(0);
    int nCohesion(0);

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);
    Real inv;

    for (size_t i(0); i < birds.size(); i++) {
        Bird const& b = birds[i];
        if (m_index != i  /*&& !canIgnore(i)*/) {
            current_distance = this->distance(b);
            if ((current_distance < SEPARATION_RANGE) && (current_distance < min_distance)) {

                if (this->insideFieldView(b)) {
                    if (m_state == state::near_predator)
                        m_state = state::near_predatorANDseparation;
                    else if (m_state == state::near_fruit)
                        m_state = state::near_fruitANDseparation;
                    else
                        m_state = state::separation;
                    min_distance = current_distance;
                    xSeparation = b.get_x();
                    ySeparation = b.get_y();
                }
            }
            else if (((m_state == state::alignment) || (m_state == state::constant)) && (current_distance > SEPARATION_RANGE) && (current_distance < ALIGNMENT_RANGE)) {

                if (this->insideFieldView(b)) {
                    m_state = state::alignment;
                    angleAlignment += b.get_angle();
                    nAlignment++;
                }
            }
            else if (((m_state == state::cohesion) || (m_state == state::constant)) && (current_distance > ALIGNMENT_RANGE) && (current_distance < COHESION_RANGE)) {

                if (this->insideFieldView(b)) {
                    m_state = state::cohesion;
                    xCohesion += b.get_x();
                    yCohesion += b.get_y();
                    nCohesion++;
                }
            }
            //(else if (current_distance > IGNORE_RANGE) {
            //    m_ignore_ids[i] = true;
            //    b.m_ignore_ids[m_index] = true;
            //}
        }
    }
    
    // choose x, y by state
    switch (m_state) {
        case state::separation:
            v = {xSeparation,ySeparation};
            break;
        case state::near_predatorANDseparation:
            v = {xSeparation,ySeparation};
            break;
        case state::alignment:
            inv = (Real) 1 / (Real)(nAlignment + 1);
            v = {angleAlignment*inv};
            break;
        case state::cohesion:
            inv = (Real) 1 / (Real)(nCohesion + 1);
            v = {xCohesion*inv,yCohesion*inv};
            break;
        default:
            v = {0};
            break;
    }

    return v;
}


std::vector<Real> Bird::neighbours_par(birds_t& birds) {
    
    std::vector<Real> v;
    Real xSeparation(0), ySeparation(0);

    Real angleAlignment(0);
    int nAlignment(0);

    Real xCohesion(0), yCohesion(0);
    int nCohesion(0);

    Real current_distance;
    Real min_distance = (Real)(WIDTH + HEIGHT);
    Real inv;

    std::vector<Real> vec_distance(birds.size());
    std::vector<bool> vec_visible(birds.size());
    
    auto compute = [&vec_distance, &vec_visible](auto const& it) {
        size_t const& i = it.first;
        Bird const& b = it.second;
        vec_distance[i] = 1.2f;//this->distance(b);
        vec_visible[i] = false; //this->insideFieldView(b);
    };

    std::for_each(std::execution::par,
        birds.begin(),
        birds.end(),
        compute
    );
    // avoiding checking of min distance for self
    vec_distance[m_index] = SIZE_MAX;
    vec_visible[m_index] =  false;
    //obtain the index of the min distance
    auto it = std::min_element(vec_distance.begin(), vec_distance.end());
    size_t min_index = std::distance(vec_distance.begin(), it);
    // size_t min_index = 0;
    if (vec_distance[min_index] < SEPARATION_RANGE && vec_visible[min_index]) {
        
        Bird const& b = birds[min_index];

        if (m_state == state::near_predator)
            m_state = state::near_predatorANDseparation;
        else if (m_state == state::near_fruit)
            m_state = state::near_fruitANDseparation;
        else
            m_state = state::separation;
        
        xSeparation = b.get_x();
        ySeparation = b.get_y();
    }
    else {
        for (size_t i(0); i < birds.size(); i++) {
            Bird const& b = birds[i];
            if (m_index != i && vec_visible[i]) {
                current_distance = vec_distance[i];
                if (((m_state == state::alignment) || (m_state == state::constant)) && (current_distance > SEPARATION_RANGE) && (current_distance < ALIGNMENT_RANGE)) {

                    m_state = state::alignment;
                    angleAlignment += b.get_angle();
                    nAlignment++;
                }
                else if (((m_state == state::cohesion) || (m_state == state::constant)) && (current_distance > ALIGNMENT_RANGE) && (current_distance < COHESION_RANGE)) {

                    m_state = state::cohesion;
                    xCohesion += b.get_x();
                    yCohesion += b.get_y();
                    nCohesion++;
                }
            }
        }
    }

    // choose x, y by state
    switch (m_state) {
    case state::separation:
        v = { xSeparation,ySeparation };
        break;
    case state::near_predatorANDseparation:
        v = { xSeparation,ySeparation };
        break;
    case state::alignment:
        inv = (Real)1 / (Real)(nAlignment + 1);
        v = { angleAlignment * inv };
        break;
    case state::cohesion:
        inv = (Real)1 / (Real)(nCohesion + 1);
        v = { xCohesion * inv,yCohesion * inv };
        break;
    default:
        v = { 0 };
        break;
    }

    return v;
}


std::vector<Real> Bird::closestPredator(predators_t& predators) {
    std::vector<Real> pred;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (auto &it : predators) {
        
        Predator& p = it.second;
        current_distance = this->distance(p);

        if ((current_distance < PREDATOR_RANGE) && (current_distance < min_distance) ) {

            if (this->insideFieldView(p)) {
                m_state = state::near_predator;
                min_distance = current_distance;
                pred={p.get_x(), p.get_y()};

                if (current_distance < DEAD_RANGE) {
                    m_alive = false;
                    break;
                }
            }
        }
    }

    return pred;
}

std::vector<Real> Bird::closestFruit(std::vector<Fruit>& fruits) {

    std::vector<Real> fr;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (auto& f : fruits) {
        // some other thread might make it false
        if (!f.get_alive())
            continue;
        current_distance = this->distance(f);

        if ( (current_distance < FRUIT_RANGE) && (current_distance<min_distance) ) {
            m_state = state::near_fruit;
            min_distance = current_distance;
            fr = {f.get_x(),f.get_y()};

            if (current_distance < DEAD_RANGE) {
                f.get_alive() = false;
            }
        }
    }
    return fr;
}

void Bird::cohesionLaw(std::vector<Real> const& group) {

    m_angle = calc::angle(COHESION_RELAXATION, group[1] - m_y, group[0] - m_x, m_angle);
}

void Bird::alignmentLaw(std::vector<Real> const& group) {
    
    m_angle = (1-ALIGNMENT_RELAXATION)*group[0] + ALIGNMENT_RELAXATION*m_angle ;
}

void Bird::biSeparationLaw(std::vector<Real> const& bird, std::vector<Real> const& predator) {
    vec2 separationBird = calc::normVector({(Real)(m_x-bird[0]),(Real)(m_y-bird[1])});
    vec2 separationPred = calc::normVector({(Real)(m_x-predator[0]),(Real)(m_y-predator[0])});

    Real angleBird = calc::angle(SEPARATION_RELAXATION, separationBird[1], separationBird[0], m_angle);
    Real anglePred = calc::angle(SEPARATION_RELAXATION, separationPred[1], separationPred[0], m_angle);

    m_angle = (Real)(0.5*anglePred + 0.5*angleBird);
}

void Bird::fruitLaw(std::vector<Real> const& f) {

    vec2 target = calc::normVector({ (float)(f[0] - m_x),(float)(f[1] - m_y) });
    m_angle = calc::angle(PREDATOR_RELAXATION, target[1], target[0], m_angle);
}

void Bird::biFruitLaw(std::vector<Real> const& f, std::vector<Real> const& bird) {

    vec2 separationBird = calc::normVector({(Real)(m_x-bird[0]),(Real)(m_y-bird[1])});
    vec2 target = calc::normVector({ (float)(f[0] - m_x),(float)(f[1] - m_y) });

    Real angleBird = calc::angle(SEPARATION_RELAXATION, separationBird[1], separationBird[0], m_angle);
    Real angleFruit = calc::angle(PREDATOR_RELAXATION, target[1], target[0], m_angle);
   
    m_angle = (Real)(0.5*angleFruit + 0.5*angleBird);
}

bool Bird::update(std::vector<Obstacle>const& obstacles, predators_t& predators, birds_t& birds, std::vector<Fruit>& fruits) {

    m_state = state::constant;
    std::vector<Real> closest_obstacle, closest_bird, closest_predator, closest_fruit;
    // Predators
    closest_predator = this->closestPredator(predators);
    // already dead then return
    if (!m_alive) {
        this->windowUpdate();
        return false;
    }

    // Obstacles
    closest_obstacle = this->closestObstacle(obstacles);

    if (m_state == state::near_obstacle) {
        this->obstacleLaw(closest_obstacle);
    }
    else {
        // Fruits
        if (m_state != state::near_predator)
            closest_fruit = this->closestFruit(fruits);

        // Neighbours
        closest_bird = this->neighbours_par(birds);

        // choose law by state and update the angle of agent
        switch (m_state) {
        case state::near_predatorANDseparation:
            this->biSeparationLaw(closest_bird, closest_predator);
            break;
        case state::near_predator:
            this->separationLaw(closest_predator);
            break;
        case state::near_fruitANDseparation:
            this->biFruitLaw(closest_fruit, closest_bird);
            break;
        case state::near_fruit:
            this->fruitLaw(closest_fruit);
            break;
        case state::separation:
            this->separationLaw(closest_bird);
            break;
        case state::alignment:
            this->alignmentLaw(closest_bird);
            break;
        case state::cohesion:
            this->cohesionLaw(closest_bird);
            break;
        }
    }
    // update the window and then set new x, y
    this->windowUpdate();
    return true;
}

Bird::~Bird() = default;

birds_t Bird::init(std::vector<Obstacle> const& obstacles, predators_t& predators) {

    birds_t birds;
    Bird bird;

    int randomX;
    int randomY;
    Real randomAngle;
    size_t n = birds.size();

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::random_device dev;
    std::mt19937 engine(dev());

    for (size_t i = 0; i<DEFAULT_NUM_BIRDS; ++i) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = (Real)(2*PI*unif(engine)-PI);

        bird = Bird((Real)randomX,(Real)randomY,randomAngle,n);
        bird.closestObstacle(obstacles);
        while (bird.get_state() == state::near_obstacle || bird.overlap(birds, predators) ) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = (Real)(2*PI*unif(engine)-PI);

            bird = Bird((Real)randomX,(Real)randomY,randomAngle,n);
            bird.closestObstacle(obstacles);
        }
        birds[n] = bird;
        n = birds.size();
    }
    return birds;
}

/*void Bird::clearIgnore(birds_t& birds) {
    
    auto apply = [](auto& it) {
        it.second.resetIgnore();
    };
    std::for_each(birds.begin(), birds.end(), apply);
}*/
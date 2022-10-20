#include "bird.h"

Bird::Bird() : Agent(), m_alive(true), m_fruit(false)
{}

Bird::Bird(Real const& x, Real const& y) : Agent(x,y), m_alive(true), m_fruit(false)
{}

Bird::Bird(Real const& x, Real const& y, Real const& angle) : Agent(x,y,angle), m_alive(true), m_fruit(false)
{}

Bird::Bird(Real const& x, Real const& y, Real const& angle, size_t& index) : Agent(x,y,angle,index), m_alive(true), m_fruit(false)
{}

bool Bird::get_alive() const {
    return m_alive;
}

std::pair<state,std::vector<Real>> Bird::neighbours(std::vector<Bird> const& birds) const {
    state s = constant;
    std::vector<Real> v;

    Real xSeparation(0), ySeparation(0);

    Real angleAlignment(0);
    int nAlignment(0);

    Real xCohesion(0), yCohesion(0);
    int nCohesion(0);

    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;
    Real inv;

    for (size_t i(0); i < birds.size(); i++) {

        if (m_index!=i) {

            current_distance = this->distance(birds[i]);

            if ( (current_distance < SEPARATION_RANGE) && (current_distance < min_distance) ) {
                if (this->insideFieldView(birds[i])) {
                    s = separation;
                    min_distance = current_distance;
                    xSeparation = birds[i].get_x();
                    ySeparation = birds[i].get_y();
                }
            }

            else if ( (s != separation) && (current_distance > SEPARATION_RANGE) && (current_distance < ALIGNMENT_RANGE) ) {
                if (this->insideFieldView(birds[i])) {
                    s = alignment;
                    angleAlignment += birds[i].get_angle();
                    nAlignment++;
                }
            }

            else if ( (s != separation) && (s != alignment) && (current_distance > ALIGNMENT_RANGE) && (current_distance < COHESION_RANGE) ) {
                if (this->insideFieldView(birds[i])) {
                    s = cohesion;
                    xCohesion += birds[i].get_x();
                    yCohesion += birds[i].get_y();
                    nCohesion++;
                }
            }
        }
    }

    switch ( s )
    {
        case separation:
            v = {xSeparation,ySeparation};
            return std::make_pair(s,v);
        case alignment:
            inv = (Real)1 / (Real)(nAlignment + 1);
            v = {angleAlignment*inv};
            return std::make_pair(s,v);
        case cohesion:
            inv = (Real)1 / (Real)(nCohesion + 1);
            v = {xCohesion*inv,yCohesion*inv};
            return std::make_pair(s,v);
        case constant:
            v = {0};
            return std::make_pair(s,v);
    }

}

std::vector<Real> Bird::pred(std::vector<Agent> const& predators) {

    std::vector<Real> pred;

    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;

    for (size_t i(0); i < predators.size(); i++) {
        current_distance = this->distance(predators[i]);

        if ( (current_distance < PREDATOR_RANGE) && (current_distance < min_distance) ) {
            if (this->insideFieldView(predators[i])) {
                min_distance = current_distance;
                pred={predators[i].get_x(), predators[i].get_y()};

                if (current_distance < DEAD_RANGE)
                    m_alive = false;
            }
        }
    }
    return pred;
}

size_t Bird::fruit(std::vector<Fruit> const& fruits) {
    m_fruit = false;
    size_t fruit_index=0;
    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;
    for (size_t i(0); i < fruits.size(); i++) {
        current_distance = this->distance(fruits[i]);
        if ( (current_distance < FRUIT_RANGE) && (current_distance<min_distance) ) {
            m_fruit = true;
            min_distance = current_distance;
            fruit_index = i;
        }
    }
    return fruit_index;
}

void Bird::cohesionLaw(std::vector<Real> const& group) {
    m_angle = (1-COHESION_RELAXATION)*atan2((group[1]-m_y),(group[0]-m_x)) + COHESION_RELAXATION*m_angle;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Bird::alignmentLaw(std::vector<Real> const& group) {
    m_angle = (1-ALIGNMENT_RELAXATION)*group[0] + ALIGNMENT_RELAXATION*m_angle ;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Bird::biSeparationLaw(std::vector<Real> const& bird, std::vector<Real> const& predator) {
    vec2 separationBird = normVector({(Real)(m_x-bird[0]),(Real)(m_y-bird[1])});
    vec2 separationPred = normVector({(Real)(m_x-predator[0]),(Real)(m_y-predator[0])});

    Real angleBird = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;
    Real anglePred = (1-SEPARATION_RELAXATION)*atan2(separationPred[1],separationPred[0]) + SEPARATION_RELAXATION*m_angle;

    m_angle = 0.5*anglePred + 0.5*angleBird;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);

}

void Bird::fruitLaw(Fruit& fruit, std::vector<Bird>& birds) {

    if (this->distance(fruit) < DEAD_RANGE) {
        size_t size = birds.size();
        birds.push_back(Bird(fruit.get_x(), fruit.get_y(), -m_angle, size));
        fruit.get_alive() = false;
    }
    else {
        vec2 target = normVector({ (float)(fruit.get_x() - m_x),(float)(fruit.get_y() - m_y) });

        m_angle = (1 - PREDATOR_RELAXATION) * atan2(target[1], target[0]) + PREDATOR_RELAXATION * m_angle;

        m_x += PRED_SPEED * cos(m_angle);
        m_y += PRED_SPEED * sin(m_angle);
    }
}

void Bird::biFruitLaw(Fruit& fruit, std::vector<Real> const& bird, std::vector<Bird>& birds) {

    if (this->distance(fruit) < DEAD_RANGE) {
        size_t size = birds.size();
        birds.push_back(Bird(fruit.get_x(), fruit.get_y(), -m_angle, size));
        fruit.get_alive() = false;
    }
    else {
        vec2 separationBird = normVector({(Real)(m_x-bird[0]),(Real)(m_y-bird[1])});
        vec2 target = normVector({ (float)(fruit.get_x() - m_x),(float)(fruit.get_y() - m_y) });

        Real angleBird = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;
        Real angleFruit = (1 - PREDATOR_RELAXATION) * atan2(target[1], target[0]) + PREDATOR_RELAXATION * m_angle;

        m_angle = m_angle = 0.5*angleFruit + 0.5*angleBird;
        m_x += PRED_SPEED * cos(m_angle);
        m_y += PRED_SPEED * sin(m_angle);
    }
}

int Bird::update(std::vector<Obstacle>const& obstacles, std::vector<Agent> const& predators, std::vector<Bird>& birds, std::vector<Fruit>& fruits) {

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<size_t> neighboursObstacle;
    size_t fruit_index;

    state s;
    std::vector<Real> update, predator;

    // Obstacles
    neighboursObstacle = this->obstacle(obstacles);
    if (m_obstacle) {
        this->obstacleLaw(obstacles, neighboursObstacle);

        this->windowUpdate();
        return 0;
    }
    else {
        // Predators
        predator = this->pred(predators);

        // Neighbours
        std::tie(s,update) = this->neighbours(birds);

        if (!predator.empty()) {
            if (s==separation)
                this->biSeparationLaw(update, predator);
            else
                this->separationLaw(predator);
            this->windowUpdate();
            return 0;

        } else {
            fruit_index = this->fruit(fruits);
            if (m_fruit) {
                if (s==separation)
                    this->biFruitLaw(fruits[fruit_index],update,birds);
                else
                    this->fruitLaw(fruits[fruit_index], birds);
                this->windowUpdate();
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                std::cout << "Fruit: " << duration.count() << std::endl;
                std::cout << "" << std::endl;
                return 0;
            }
            else if (s==separation) {
                this->separationLaw(update);
                this->windowUpdate();
                return 0;
            }
            else if (s==alignment) {
                this->alignmentLaw(update);
                this->windowUpdate();
                return 0;
            }
            else if (s==cohesion) {
                this->cohesionLaw(update);
                this->windowUpdate();
                return 0;
            }
            else {
                this->constantUpdate();
                this->windowUpdate();
                return 0;
            }
        }
    }
}

Bird::~Bird() {

}

std::vector<Agent> birds2agents(std::vector<Bird> const& birds) {
    std::vector<Agent> agents(birds.size());
    for (Bird const& bird : birds) {
        agents.push_back(Agent(bird));
    }
    return agents;
}

std::vector<Bird> birds_init(std::vector<Obstacle> const& obstacles, std::vector<Agent> const& predators) {

    std::vector<Bird> birds;
    Bird bird;

    int randomX;
    int randomY;
    Real randomAngle;
    size_t n = birds.size();

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::random_device dev;
    std::mt19937 engine(dev());

    for (size_t i = 0; i<DEFAULT_NUM_BIRDS; ++i) {

        randomX = uniX(engine);
        randomY = uniY(engine);
        randomAngle = 2*PI*unif(engine)-PI;

        bird = Bird(randomX,randomY,randomAngle,n);
        bird.obstacle(obstacles);
        while (bird.get_obstacle() || bird.overlap(birds2agents(birds)) || bird.overlap(predators) ) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = 2*PI*unif(engine)-PI;

            bird = Bird(randomX,randomY,randomAngle,n);
            bird.obstacle(obstacles);
        }
        birds.push_back(bird);
        n = birds.size();
    }
    return birds;
}

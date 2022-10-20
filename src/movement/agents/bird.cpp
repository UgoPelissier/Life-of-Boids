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

bool Bird::get_fruit() const {
    return m_fruit;
}

std::vector<std::vector<size_t>> Bird::neighbours(std::vector<Bird> const& birds) const {
    std::vector<size_t> separation, alignment, cohesion;
    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;
    for (size_t i(0); i < birds.size(); i++) {
        if (m_index!=i && this->insideFieldView(birds[i])) {
            current_distance = this->distance(birds[i]);
            if ( (current_distance < SEPARATION_RANGE) && (current_distance < min_distance) ) {
                min_distance = current_distance;
                separation={i};
            }
            else if (current_distance > SEPARATION_RANGE && current_distance < ALIGNMENT_RANGE)
                alignment.push_back(i);
            else if (current_distance > ALIGNMENT_RANGE && current_distance < COHESION_RANGE)
                cohesion.push_back(i);
        }
    }
    return {separation, alignment, cohesion};
}

std::vector<size_t> Bird::pred(std::vector<Agent> const& predators) {
    std::vector<size_t> preds;
    Real current_distance;
    Real min_distance = WIDTH+HEIGHT;
    for (size_t i(0); i < predators.size(); i++) {
        if (this->insideFieldView(predators[i])) {
            current_distance = this->distance(predators[i]);
            if ( (current_distance < PREDATOR_RANGE) && (current_distance < min_distance) ) {
                min_distance = current_distance;
                preds={i};
                if (current_distance < DEAD_RANGE)
                    m_alive = false;
            }
        }
    }
    return preds;
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

void Bird::cohesionLaw(std::vector<Bird> const& birds, std::vector<size_t> const& neighbours) {

    vec3 v = this->center(birds2agents(birds), neighbours);

    m_angle = (1-COHESION_RELAXATION)*atan2((v[1]-m_y),(v[0]-m_x)) + COHESION_RELAXATION*m_angle;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Bird::alignmentLaw(std::vector<Bird> const& birds, std::vector<size_t> const& neighbours) {

    vec3 v = this->center(birds2agents(birds), neighbours);

    m_angle = (1-ALIGNMENT_RELAXATION)*v[2] + ALIGNMENT_RELAXATION*m_angle ;
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Bird::separationLaw(Bird const& bird){
    vec2 separationBird = normVector({(Real)(m_x-bird.get_x()),(Real)(m_y-bird.get_y())});

    m_angle = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Bird::separationLaw(Agent const&  predator) {
    vec2 separationPred = normVector({(Real)(m_x-predator.get_x()),(Real)(m_y-predator.get_y())});

    m_angle = (1-SEPARATION_RELAXATION)*atan2(separationPred[1],separationPred[0]) + SEPARATION_RELAXATION*m_angle;

    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);
}

void Bird::biSeparationLaw(Bird const& bird, Agent const&  predator) {
    vec2 separationBird = normVector({(Real)(m_x-bird.get_x()),(Real)(m_y-bird.get_y())});
    vec2 separationPred = normVector({(Real)(m_x-predator.get_x()),(Real)(m_y-predator.get_y())});

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

void Bird::biFruitLaw(Fruit& fruit, Bird const& bird, std::vector<Bird>& birds) {

    if (this->distance(fruit) < DEAD_RANGE) {
        size_t size = birds.size();
        birds.push_back(Bird(fruit.get_x(), fruit.get_y(), -m_angle, size));
        fruit.get_alive() = false;
    }
    else {
        vec2 separationBird = normVector({(Real)(m_x-bird.get_x()),(Real)(m_y-bird.get_y())});
        vec2 target = normVector({ (float)(fruit.get_x() - m_x),(float)(fruit.get_y() - m_y) });

        Real angleBird = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;
        Real angleFruit = (1 - PREDATOR_RELAXATION) * atan2(target[1], target[0]) + PREDATOR_RELAXATION * m_angle;

        m_angle = m_angle = 0.5*angleFruit + 0.5*angleBird;
        m_x += PRED_SPEED * cos(m_angle);
        m_y += PRED_SPEED * sin(m_angle);
    }
}

void Bird::update(std::vector<Obstacle>const& obstacles, std::vector<Agent> const& predators, std::vector<Bird>& birds, std::vector<Fruit>& fruits) {

    std::vector<std::vector<size_t>> v;
    std::vector<size_t> predator, separation, alignment, cohesion, neighboursObstacle;
    size_t fruit_index;

    // Obstacles
    neighboursObstacle = this->obstacle(obstacles);
    if (m_obstacle)
        this->obstacleLaw(obstacles, neighboursObstacle);
    else {
        // Predators
        predator = this->pred(predators);

        // Neighbours
        v = this->neighbours(birds);
        separation = v[0];
        alignment = v[1];
        cohesion = v[2];

        if (!predator.empty()) {
            if (!separation.empty())
                this->biSeparationLaw(birds[separation[0]], predators[predator[0]]);
            else
                this->separationLaw(predators[predator[0]]);
        } else {
            fruit_index = this->fruit(fruits);
            if (m_fruit) {
                if (!separation.empty())
                    this->biFruitLaw(fruits[fruit_index],birds[separation[0]],birds);
                else
                    this->fruitLaw(fruits[fruit_index], birds);
            }
            else if (!separation.empty())
                this->separationLaw(birds[separation[0]]);
            else if (!alignment.empty())
                this->alignmentLaw(birds, alignment);
            else if (!cohesion.empty())
                this->cohesionLaw(birds, cohesion);
            else
                this->constantUpdate();
        }
        this->windowUpdate();
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

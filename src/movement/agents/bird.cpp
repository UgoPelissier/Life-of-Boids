#include "bird.h"

Bird::Bird() : Agent(), m_alive(true)
{}

Bird::Bird(Real const& x, Real const& y) : Agent(x,y), m_alive(true)
{}

Bird::Bird(Real const& x, Real const& y, Real const& angle) : Agent(x,y,angle), m_alive(true)
{}

Bird::Bird(Real const& x, Real const& y, Real const& angle, size_t& index) : Agent(x,y,angle,index), m_alive(true)
{}

bool Bird::get_alive() const {
    return m_alive;
}

std::vector<Real> Bird::neighbours(std::vector<Bird> const& birds) {
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

        if (m_index!=i) {

            current_distance = this->distance(birds[i]);

            if ( (current_distance < SEPARATION_RANGE) && (current_distance < min_distance) ) {
                if (this->insideFieldView(birds[i])) {
                    if ( m_state==predator || m_state==predatorANDseparation )
                        m_state = predatorANDseparation;
                    else if ( m_state==fruit || m_state==fruitANDseparation )
                        m_state = fruitANDseparation;
                    else
                        m_state = separation;
                    min_distance = current_distance;
                    xSeparation = birds[i].get_x();
                    ySeparation = birds[i].get_y();
                }
            }
            else if ( ( (m_state == alignment) || (m_state == constant) ) && (current_distance > SEPARATION_RANGE) && (current_distance < ALIGNMENT_RANGE) ) {
                //if ( (current_distance > SEPARATION_RANGE) && (current_distance < ALIGNMENT_RANGE) ) {
                    if (this->insideFieldView(birds[i])) {
                        m_state = alignment;
                        angleAlignment += birds[i].get_angle();
                        nAlignment++;
                    }
                //}
            }
            else if ( ( (m_state == cohesion) || (m_state == constant) ) && (current_distance > ALIGNMENT_RANGE) && (current_distance < COHESION_RANGE) ) {
                //if ((current_distance > ALIGNMENT_RANGE) && (current_distance < COHESION_RANGE)) {
                    if (this->insideFieldView(birds[i])) {
                        m_state = cohesion;
                        xCohesion += birds[i].get_x();
                        yCohesion += birds[i].get_y();
                        nCohesion++;
                    }
                //}
            }
        }
    }

    switch ( m_state )
    {
        case separation:
            v = {xSeparation,ySeparation};
            return v;
        case predatorANDseparation:
            v = {xSeparation,ySeparation};
            return v;
        case alignment:
            inv = (Real)1 / (Real)(nAlignment + 1);
            v = {angleAlignment*inv};
            return v;
        case cohesion:
            inv = (Real)1 / (Real)(nCohesion + 1);
            v = {xCohesion*inv,yCohesion*inv};
            return v;
        default:
            v = {0};
            return v;
    }
}

std::vector<Real> Bird::pred(std::vector<Agent> const& predators) {
    std::vector<Real> pred;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (const auto & p : predators) {
        current_distance = this->distance(p);

        if ( (current_distance < PREDATOR_RANGE) && (current_distance < min_distance) ) {

            if (this->insideFieldView(p)) {
                m_state = predator;
                min_distance = current_distance;
                pred={p.get_x(), p.get_y()};

                if (current_distance < DEAD_RANGE)
                    m_alive = false;
            }
        }
    }
    return pred;
}

std::vector<Real> Bird::fruits(std::vector<Fruit>& fruits, std::vector<Bird>& birds) {

    std::vector<Real> fr;

    Real current_distance;
    Real min_distance = (Real)(WIDTH+HEIGHT);

    for (auto & f : fruits) {
        current_distance = this->distance(f);

        if ( (current_distance < FRUIT_RANGE) && (current_distance<min_distance) ) {
            m_state = fruit;
            min_distance = current_distance;
            fr = {f.get_x(),f.get_y()};

            if (current_distance < DEAD_RANGE) {
                size_t size = birds.size();
                // birds.emplace_back(f.get_x(), f.get_y(), -m_angle, size);
                f.get_alive() = false;
            }
        }
    }
    return fr;
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

    m_angle = (Real)(0.5*anglePred + 0.5*angleBird);
    m_x += SPEED * cos(m_angle);
    m_y += SPEED * sin(m_angle);

}

void Bird::fruitLaw(std::vector<Real> const& f) {

    vec2 target = normVector({ (float)(f[0] - m_x),(float)(f[1] - m_y) });

    m_angle = (1 - PREDATOR_RELAXATION) * atan2(target[1], target[0]) + PREDATOR_RELAXATION * m_angle;

    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

void Bird::biFruitLaw(std::vector<Real> const& f, std::vector<Real> const& bird) {

    vec2 separationBird = normVector({(Real)(m_x-bird[0]),(Real)(m_y-bird[1])});
    vec2 target = normVector({ (float)(f[0] - m_x),(float)(f[1] - m_y) });

    Real angleBird = (1-SEPARATION_RELAXATION)*atan2(separationBird[1],separationBird[0]) + SEPARATION_RELAXATION*m_angle;
    Real angleFruit = (1 - PREDATOR_RELAXATION) * atan2(target[1], target[0]) + PREDATOR_RELAXATION * m_angle;

    m_angle = m_angle = (Real)(0.5*angleFruit + 0.5*angleBird);
    m_x += PRED_SPEED * cos(m_angle);
    m_y += PRED_SPEED * sin(m_angle);
}

int Bird::update(std::vector<Obstacle>const& obstacles, std::vector<Agent> const& predators, std::vector<Bird>& birds, std::vector<Fruit>& fruits) {

    m_state = constant;
    std::vector<Real> update, pred, f;

    // Obstacles
    update = this->obstacle(obstacles);

    if (m_state==obst) {
        this->obstacleLaw(update);
        this->windowUpdate();
        return 0;
    }

    // Predators
    pred = this->pred(predators);

    // Fruits
    if ( m_state!= predator )
        f = this->fruits(fruits,birds);

    // Neighbours
    update = this->neighbours(birds);

    switch ( m_state )
    {
        case predatorANDseparation:
            this->biSeparationLaw(update, pred);
            this->windowUpdate();
            return 0;
        case predator:
            this->separationLaw(pred);
            this->windowUpdate();
            return 0;
        case fruitANDseparation:
            this->biFruitLaw(f,update);
            this->windowUpdate();
            return 0;
        case fruit:
            this->fruitLaw(f);
            this->windowUpdate();
            return 0;
        case separation:
            this->separationLaw(update);
            this->windowUpdate();
            return 0;
        case alignment:
            this->alignmentLaw(update);
            this->windowUpdate();
            return 0;
        case cohesion:
            this->cohesionLaw(update);
            this->windowUpdate();
            return 0;
        case constant:
            this->constantUpdate();
            this->windowUpdate();
            return 0;
        default:
            this->constantUpdate();
            this->windowUpdate();
            return 0;
    }
}

Bird::~Bird() = default;

std::vector<Agent> birds2agents(std::vector<Bird> const& birds) {
    std::vector<Agent> agents(birds.size());
    for (Bird const& bird : birds) {
        agents.emplace_back(bird);
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
        randomAngle = (Real)(2*PI*unif(engine)-PI);

        bird = Bird((Real)randomX,(Real)randomY,randomAngle,n);
        bird.obstacle(obstacles);
        while (bird.get_state()==obst || bird.overlap(birds2agents(birds)) || bird.overlap(predators) ) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomAngle = (Real)(2*PI*unif(engine)-PI);

            bird = Bird((Real)randomX,(Real)randomY,randomAngle,n);
            bird.obstacle(obstacles);
        }
        birds.push_back(bird);
        n = birds.size();
    }
    return birds;
}

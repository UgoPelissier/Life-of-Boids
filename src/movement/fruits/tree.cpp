#include "tree.h"

Tree::Tree() : Object(), m_height(0), m_width(0), m_obstacle(false), m_time(0)
{}

Tree::Tree(Real const& x, Real const& y, Real const& height, Real const& width, double const& time) : Object(x,y), m_height(height), m_width(width), m_obstacle(false), m_time(time)
{}

Real Tree::get_height() const {
    return m_height;
}

Real Tree::get_width() const {
    return m_width;
}

bool Tree::get_obstacle() const {
    return m_obstacle;
}

bool Tree::borders() const {
    if (m_x < (Real)CLOSE) {
        return true;
    }
    else if (( (Real)WIDTH- (Real)CLOSE < m_x) && (m_x < (Real)WIDTH)) {
        return true;
    }
    else if (m_y < CLOSE) {
        return true;
    }
    else if (( (Real)HEIGHT- (Real)CLOSE ) < m_y && (m_y < (Real)HEIGHT) ) {
        return true;
    }
    return false;
}

void Tree::DropFruitAndAppend(std::vector<Fruit>& fruits, std::vector<Obstacle> const& obstacles) {
    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniNumFruit(1, DEFAULT_NUM_FRUITS_DROPS);
    std::random_device dev;
    std::mt19937 engine(dev());
    time_t finish;

    Fruit fruit;

    if (m_time<=(double)time(&finish)) {
        int randomNumFruit;
        Real randomX;
        Real randomY;
        std::uniform_int_distribution uniTime(FRUIT_TIME_MIN, FRUIT_TIME_MAX);

        randomNumFruit = uniNumFruit(engine);

        for (int i(0); i < randomNumFruit; i++) {
            std::uniform_real_distribution<Real> uniX(0, MAX_FRUIT_DISTANCE);
            std::uniform_real_distribution<Real> uniY(0, MAX_FRUIT_DISTANCE);
            randomX = m_x + uniX(engine);
            randomY = m_y + uniY(engine);

            fruit = Fruit(randomX, randomY);
            fruit.obstacle(obstacles);
            while (fruit.borders() || fruit.get_obstacle()) {
                randomX = uniX(engine);
                randomY = uniY(engine);
                fruit = Fruit(randomX, randomY);
                fruit.obstacle(obstacles);
            }
            fruits.emplace_back(randomX, randomY, std::min(m_height,m_width)/4, true);
        }
        m_time = (double)uniTime(engine) + (double)time(&finish);
    }
}

void Tree::obstacle(std::vector<Obstacle> const& obstacles) {
    m_obstacle = false;
    for (const auto & obstacle : obstacles) {
        if ( this->distance(obstacle) < std::max(obstacle.get_height()/2,obstacle.get_width()/2) ) {
            m_obstacle = true;
        }
    }
}

Tree::~Tree() = default;

std::vector<Tree> Tree::init(std::vector<Obstacle> const& obstacles) {
    std::vector<Tree> trees;
    Tree newTree;

    Real randomX;
    Real randomY;
    int randomHeight;
    int randomWidth;
    double randomTime;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_real_distribution<Real> uniX(0, (Real)WIDTH);
    std::uniform_real_distribution<Real> uniY(0, (Real)HEIGHT);
    std::uniform_int_distribution uniSize(MIN_FRUIT_TREE_SIZE, MAX_FRUIT_TREE_SIZE);
    std::uniform_int_distribution uniTime(FRUIT_TIME_MIN, FRUIT_TIME_MAX);
    std::random_device dev;
    std::mt19937 engine(dev());
    time_t start;

    while (trees.size() < DEFAULT_NUM_TREES) {
        randomX = uniX(engine);
        randomY = uniY(engine);
        randomHeight = uniSize(engine);
        randomWidth = uniSize(engine);
        randomTime = (double)uniTime(engine)+(double)time(&start);
        newTree = Tree((Real)randomX, (Real)randomY, (Real)randomHeight, (Real)randomWidth, randomTime);

        newTree.obstacle(obstacles);
        while (newTree.borders() || newTree.get_obstacle()) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomHeight = uniSize(engine);
            randomWidth = uniSize(engine);
            newTree = Tree((Real)randomX, (Real)randomY, (Real)randomHeight, (Real)randomWidth, randomTime);
            newTree.obstacle(obstacles);
        }

        trees.push_back(newTree);
    }
    return trees;
}
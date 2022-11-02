#include "tree.h"

Tree::Tree() : Eco(), m_height(0), m_width(0), m_time(0) {}
Tree::Tree(Real const& x, Real const& y, Real const& height, Real const& width, double const& time) : Eco(x,y), m_height(height), m_width(width), m_time(time) {}

Real Tree::get_height() const {
    return m_height;
}

Real Tree::get_width() const {
    return m_width;
}

void Tree::DropFruitAndAppend(std::vector<Fruit>& fruits, std::vector<Obstacle> const& obstacles) {

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniNumFruit(1, DEFAULT_NUM_FRUITS_DROPS);
    std::uniform_int_distribution uniTime(FRUIT_TIME_MIN, FRUIT_TIME_MAX);
    std::random_device dev;
    std::mt19937 engine(dev());
    time_t finish;

    Fruit fruit;

    if (m_time<=(double)time(&finish)) {
        int randomNumFruit;
        Real randomX;
        Real randomY;

        randomNumFruit = uniNumFruit(engine);

        for (int i(0); i < randomNumFruit; i++) {
            std::uniform_real_distribution<Real> uniX(0, MAX_FRUIT_DISTANCE);
            std::uniform_real_distribution<Real> uniY(0, MAX_FRUIT_DISTANCE);
            randomX = m_x + uniX(engine);
            randomY = m_y + uniY(engine);

            fruit = Fruit(randomX, randomY);
            fruit.closestObstacle(obstacles);
            while (fruit.nearBorder() || fruit.get_obstacle()) {
                randomX = m_x + uniX(engine);
                randomY = m_y + uniY(engine);
                fruit = Fruit(randomX, randomY);
                fruit.closestObstacle(obstacles);
            }
            std::cout << "Fruit X : " << randomX << " ; Fruit Y : " << randomY << std::endl;
            fruits.emplace_back(randomX, randomY, std::min(m_height,m_width)/4, true);
        }
        m_time = (double)uniTime(engine) + (double)time(&finish);
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

        newTree.closestObstacle(obstacles);
        while (newTree.nearBorder() || newTree.get_obstacle()) {
            randomX = uniX(engine);
            randomY = uniY(engine);
            randomHeight = uniSize(engine);
            randomWidth = uniSize(engine);
            newTree = Tree((Real)randomX, (Real)randomY, (Real)randomHeight, (Real)randomWidth, randomTime);
            newTree.closestObstacle(obstacles);
        }

        trees.push_back(newTree);
    }
    return trees;
}
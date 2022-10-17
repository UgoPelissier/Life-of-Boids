#include "fruit.h"
#include "agent.hpp"
#include "obstacle.h"

Fruit::Fruit() {
    m_x = 0;
    m_y = 0;
    m_height = 0;
    m_width = 0;
    m_alive = true;
}

Fruit::Fruit(Real const& x, Real const& y, Real const& height, Real const& width, bool const& alive) {
    m_x = x;
    m_y = y;
    m_height = height;
    m_width = width;
    m_alive = alive;
}

Real& Fruit::get_x() {
    return m_x;
}

Real& Fruit::get_y() {
    return m_y;
}

Real& Fruit::get_height() {
    return m_height;
}

Real& Fruit::get_width() {
    return m_width;
}

bool& Fruit::get_alive() {
    return m_alive;
}

FruitTree::FruitTree() {
    m_x = 0;
    m_y = 0;
    m_height = 0;
    m_width = 0;
    m_time = 0;
}

FruitTree::FruitTree(Real const& x, Real const& y, Real const& height, Real const& width, double const& time) {
    m_x = x;
    m_y = y;
    m_height = height;
    m_width = width;
    m_time = time;
}

Real& FruitTree::get_x() {
    return m_x;
}

Real& FruitTree::get_y() {
    return m_y;
}

Real& FruitTree::get_height() {
    return m_height;
}

Real& FruitTree::get_width() {
    return m_width;
}

double FruitTree::get_time() {
    return m_time;
}

bool FruitTree::borders() {
    if (m_x < CLOSE) {
        return true;
    }
    else if ((WIDTH - CLOSE < m_x) && (m_x < WIDTH)) {
        return true;
    }
    else if (m_y < CLOSE) {
        return true;
    }
    else if ((HEIGHT - CLOSE < m_y) && (m_y < HEIGHT)) {
        return true;
    }
    return false;
}

FruitTree randomFruitTree() {
    FruitTree fruit_tree;

    int randomX;
    int randomY;
    int randomHeight;
    int randomWidth;
    double randomTime;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::uniform_int_distribution uniSize(MIN_FRUIT_TREE_SIZE, MAX_FRUIT_TREE_SIZE);
    //   std::uniform_int_distribution uniTime(FRUIT_TIME_MIN, FRUIT_TIME_MAX);
    std::mt19937_64 rng;
    //   time_t start;

    randomX = uniX(rng);
    randomY = uniY(rng);
    randomHeight = uniSize(rng);
    randomWidth = uniSize(rng);
    randomTime = 5;
    //   randomTime = uniTime(rng) + time(&start);

    fruit_tree = FruitTree(randomX, randomY, randomHeight, randomWidth, randomTime);

    return fruit_tree;
}

std::vector<FruitTree> initFruitTree(std::vector<Obstacle> obstacles) {
    std::vector<FruitTree> fruit_trees;
    FruitTree newFruitTree;

    int randomX;
    int randomY;
    int randomHeight;
    int randomWidth;
    double randomTime;

    Agent agent;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniX(0, WIDTH);
    std::uniform_int_distribution uniY(0, HEIGHT);
    std::uniform_int_distribution uniSize(MIN_FRUIT_TREE_SIZE, MAX_FRUIT_TREE_SIZE);
    std::uniform_int_distribution uniTime(FRUIT_TIME_MIN, FRUIT_TIME_MAX);
    std::mt19937_64 rng;
    time_t start;

    while (fruit_trees.size() < DEFAULT_NUM_FRUIT_TREES) {
        randomX = uniX(rng);
        randomY = uniY(rng);
        randomHeight = uniSize(rng);
        randomWidth = uniSize(rng);
        randomTime = uniTime(rng)+time(&start);
        newFruitTree = FruitTree(randomX, randomY, randomHeight, randomWidth, randomTime);

        agent = Agent(randomX, randomY);
        agent.obstacle(obstacles);
        while (newFruitTree.borders() || agent.get_obstacle()) {
            randomX = uniX(rng);
            randomY = uniY(rng);
            randomHeight = uniSize(rng);
            randomWidth = uniSize(rng);
            newFruitTree = FruitTree(randomX, randomY, randomHeight, randomWidth, randomTime);

            agent = Agent(randomX, randomY);
            agent.obstacle(obstacles);

        }

        fruit_trees.push_back(newFruitTree);
    }
    return fruit_trees;
}

std::vector<Fruit> FruitTree::DropFruit(std::vector<Fruit>& fruits) {
    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::uniform_int_distribution uniNumFruit(DEFAULT_NUM_FRUITS_DROPS/2, DEFAULT_NUM_FRUITS_DROPS);
    std::mt19937_64 rng;
    time_t finish;

    if (m_time<=time(&finish)) {
        int randomNumFruit;
        int randomX;
        int randomY;
        std::uniform_int_distribution uniTime(FRUIT_TIME_MIN, FRUIT_TIME_MAX);

        randomNumFruit = uniNumFruit(rng);

        for (int i(0); i < randomNumFruit; i++) {
            std::uniform_int_distribution uniX(0, MAX_FRUIT_DISTANCE * 2);
            std::uniform_int_distribution uniY(0, MAX_FRUIT_DISTANCE * 2);
            randomX = uniX(rng) - MAX_FRUIT_DISTANCE + (int)m_x;
            randomY = uniY(rng) - MAX_FRUIT_DISTANCE + (int)m_y;
            fruits.push_back(Fruit(randomX, randomY, m_height / 2, m_width / 2, true));
        }
        m_time= uniTime(rng) + time(&finish);
    }
    return fruits;
}

#include <iostream>
#include "gtest/gtest.h"
#include "../src/display/display.h"

bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

// blank namespace to avoid implementation collision
namespace {

TEST(FirstGTest, ExpectCanary) {
  EXPECT_EQ(1, 1) << "Canary could not fail!";
}

TEST(Initialization, VectorSize) {

    std::vector<Obstacle> obstacles = obstacles_init();
    std::vector<Agent> predators = predators_init(obstacles);
    std::vector<Bird> birds = birds_init(obstacles, predators);
    std::vector<Tree> trees = trees_init(obstacles);

    std::vector<std::array<triangle::Vertex, 3>> trianglesPredators;
    std::vector<std::array<triangle::Vertex, 3>> trianglesBirds;

    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::vector<std::array<triangle::Vertex, 3>> obstacle;

    std::vector<std::array<triangle::Vertex, 3>> trianglesTree;
    std::vector<std::array<triangle::Vertex, 3>> tree_triangles;

    for (Agent const& predator : predators) {
        trianglesPredators.push_back(triangle::newTriangle(scale(predator), PRED_COLOR, predator.get_angle(), 2 * BODY_SIZE));
        }
    for (Bird const& bird : birds) {
        trianglesBirds.push_back(triangle::newTriangle(scale(bird), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
        }
    for (Obstacle const& obs : obstacles) {
        obstacle = triangle::newObstacle(scale(obs), OBSTACLE_COLOR, obs.get_height()/HEIGHT, obs.get_width()/WIDTH);
        for (const auto & i : obstacle) {
            trianglesObs.push_back(i);
            }
        }
    for (Tree const& tree : trees) {
        tree_triangles = triangle::newTree(scale(tree), TREE_COLOR, tree.get_height() / HEIGHT, tree.get_width() / WIDTH);
        for (const auto & i : tree_triangles) {
                trianglesTree.push_back(i);
            }
    }

    EXPECT_EQ(obstacles.size(), DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";
    EXPECT_EQ(trianglesObs.size(), 2*DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";

    EXPECT_EQ(predators.size(), DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";
    EXPECT_EQ(trianglesPredators.size(), DEFAULT_NUM_PREDATORS) << "Not the correct number of obstacles!";

    EXPECT_EQ(birds.size(), DEFAULT_NUM_BIRDS) << "Not the correct number of agents!";
    EXPECT_EQ(trianglesBirds.size(), DEFAULT_NUM_BIRDS) << "Not the correct number of obstacles!";

    EXPECT_EQ(trees.size(), DEFAULT_NUM_TREES) << "Not the correct number of obstacles!";
    EXPECT_EQ(trianglesTree.size(), 2*DEFAULT_NUM_TREES) << "Not the correct number of obstacles!";
}

TEST(Initialization, Position) {

    std::vector<Obstacle> obstacles = obstacles_init();
    std::vector<Agent> predators = predators_init(obstacles);
    std::vector<Bird> birds = birds_init(obstacles, predators);

    std::vector<std::array<triangle::Vertex, 3>> trianglesBirds;

    for (Bird const& bird : birds) {
        trianglesBirds.push_back(triangle::newTriangle(scale(bird), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
    }

    int N = DEFAULT_NUM_BIRDS;
    std::uniform_int_distribution uni(0, N); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    int i = uni(engine);

    vec2 center = scale(birds[i]);

    EXPECT_NEAR(center[0], triangle::center(trianglesBirds[i])[0],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    EXPECT_NEAR(center[1], triangle::center(trianglesBirds[i])[1],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    }

}  // namespace
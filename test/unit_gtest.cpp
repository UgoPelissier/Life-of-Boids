#include <iostream>
#include "gtest/gtest.h"
#include "../src/movement/agent.hpp"
#include "../src/display/display.h"

bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

// blank namespace to avoid implementation collision
namespace {

TEST(FirstGTest, ExpectCanary) {
  // EXPECT_* is a weak assertion: should be true or report an error
  EXPECT_EQ(1, 1) << "Canary could not fail!";  // with an additional message

  // this point will be executed even if an EXPECT failed before
}

TEST(Initialization, VectorSize) {

    std::vector<Obstacle> obstacles = initObstacles();
    std::vector<Agent> agents = initialiaze_agents(obstacles);

    std::vector<std::array<triangle::Vertex, 3>> triangles;
    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::vector<std::array<triangle::Vertex, 3>> obstacle;

    for (Agent agent : agents) {
        if ( agent.get_predator() )
            triangles.push_back(triangle::newTriangle(scale(agent), PRED_COLOR, agent.get_angle(), 2 * BODY_SIZE));
        else
            triangles.push_back(triangle::newTriangle(scale(agent), BIRD_COLOR, agent.get_angle(), BODY_SIZE));
    }
    for (Obstacle obs : obstacles) {
        obstacle = triangle::newObstacle(scale(obs), OBSTACLE_COLOR, obs.get_height()/HEIGHT, obs.get_width()/WIDTH);
        trianglesObs.push_back(obstacle[0]);
        trianglesObs.push_back(obstacle[1]);
    }

    EXPECT_EQ(agents.size(), DEFAULT_NUM_AGENTS+DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";  // with an additional message
    EXPECT_EQ(triangles.size(), DEFAULT_NUM_AGENTS+DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";  // with an additional message
    EXPECT_EQ(obstacles.size(), DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";  // with an additional message
    EXPECT_EQ(trianglesObs.size(), 2*DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";  // with an additional message
}

TEST(Initialization, Position) {

    std::vector<Obstacle> obstacles = initObstacles();
    std::vector<Agent> agents = initialiaze_agents(obstacles);

    std::vector<std::array<triangle::Vertex, 3>> triangles;
    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::vector<std::array<triangle::Vertex, 3>> obstacle;

    for (Agent agent : agents) {
        if ( agent.get_predator() )
            triangles.push_back(triangle::newTriangle(scale(agent), PRED_COLOR, agent.get_angle(), 2 * BODY_SIZE));
        else
            triangles.push_back(triangle::newTriangle(scale(agent), BIRD_COLOR, agent.get_angle(), BODY_SIZE));
    }
    for (Obstacle obs : obstacles) {
        obstacle = triangle::newObstacle(scale(obs), OBSTACLE_COLOR, obs.get_height()/HEIGHT, obs.get_width()/WIDTH);
        trianglesObs.push_back(obstacle[0]);
        trianglesObs.push_back(obstacle[1]);
    }

    int N = DEFAULT_NUM_AGENTS+DEFAULT_NUM_PREDATORS;
    std::uniform_int_distribution uni(0, N); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    int i = uni(engine);

    vec2 center = scale(agents[i]);

    EXPECT_NEAR(center[0], triangle::center(triangles[i])[0],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    EXPECT_NEAR(center[1], triangle::center(triangles[i])[1],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    }

}  // namespace
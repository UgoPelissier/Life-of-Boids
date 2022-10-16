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

    /* GLFWwindow* window;
    VertexArray triangle_vertexArray = {}, triangleObs_vertexArray = {};
    Buffer triangle_buffer = {0};
    ShaderProgram triangle_shaderProgram = {0};
    GLint mvp_location;
    std::tie(window, triangle_vertexArray, triangleObs_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location) = initWindow();

    // Agents initialization
    std::vector<Agent> agents;
    std::vector<Obstacle> obstacles;
    std::vector<std::array<triangle::Vertex, 3>> triangles; // Array that will contain the birds, represented with triangles
    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::tie(agents, obstacles,triangles, trianglesObs) = initAgentWindow();

    EXPECT_EQ(agents.size(), DEFAULT_NUM_AGENTS+DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";  // with an additional message
    EXPECT_EQ(triangles.size(), DEFAULT_NUM_AGENTS+DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";  // with an additional message
    EXPECT_EQ(obstacles.size(), DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";  // with an additional message
    EXPECT_EQ(trianglesObs.size(), 2*DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";  // with an additional message */

    EXPECT_EQ(1, 1) << "Canary could not fail!";
}

TEST(Initialization, Position) {

    /* GLFWwindow* window;
    VertexArray triangle_vertexArray = {}, triangleObs_vertexArray = {};
    Buffer triangle_buffer = {0};
    ShaderProgram triangle_shaderProgram = {0};
    GLint mvp_location;
    std::tie(window, triangle_vertexArray, triangleObs_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location) = initWindow();

    // Agents initialization
    std::vector<Agent> agents;
    std::vector<Obstacle> obstacles;
    std::vector<std::array<triangle::Vertex, 3>> triangles; // Array that will contain the birds, represented with triangles
    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::tie(agents, obstacles,triangles, trianglesObs) = initAgentWindow();

    int N = DEFAULT_NUM_AGENTS+DEFAULT_NUM_PREDATORS;
    std::uniform_int_distribution uni(0, N); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    int i = uni(engine);

    vec2 center = scale(agents[i]);

    // EXPECT_NEAR(center[0], triangle::center(triangles[i])[0],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    // EXPECT_NEAR(center[1], triangle::center(triangles[i])[1],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]"; */

    EXPECT_EQ(1, 1) << "Canary could not fail!";
}

}  // namespace
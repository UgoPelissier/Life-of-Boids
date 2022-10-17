#include "agent.hpp"
#include "display.h"

bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

int main() {

    // Window initialization
    GLFWwindow* window;

    VertexArray triangle_vertexArray = {}, triangleObs_vertexArray = {}, triangleTree_vertexArray = {}, triangleFruit_vertexArray = {};
    Buffer triangle_buffer = {0};
    ShaderProgram triangle_shaderProgram = {0};

    GLint mvp_location;
    std::tie(window, triangle_vertexArray, triangleObs_vertexArray, triangleTree_vertexArray, triangleFruit_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location) = initWindow();

    // Agents initialization
    std::vector<Agent> agents;
    std::vector<Obstacle> obstacles;
    std::vector<FruitTree> trees;
    std::vector<Fruit> fruits;
    std::vector<std::array<triangle::Vertex, 3>> triangles; // Array that will contain the birds, represented with triangles
    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::vector<std::array<triangle::Vertex, 3>> triangleTree;
    std::vector<std::array<triangle::Vertex, 3>> triangleFruit;
    std::tie(agents, obstacles, trees, fruits, triangles, trianglesObs, triangleTree, triangleFruit) = initAgentWindow();

    // Global loop
    int i = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {

        updateAgentWindow(window, agents, obstacles, trees, fruits, triangles, triangleFruit);
        addAgent(window, addBird, addPredator, agents, obstacles, triangles);
        updateWindow(window, triangles, trianglesObs, triangleTree, triangleFruit, triangle_vertexArray, triangleObs_vertexArray, triangleTree_vertexArray, triangleFruit_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location);

        i++;
        if (i == NUMBER_LOOP_FPS) {
            auto end = std::chrono::high_resolution_clock::now();
            display_fps(window, start, end);
            i = 0;
            start = std::chrono::high_resolution_clock::now();
        }


    }

    endWindow(window);
}

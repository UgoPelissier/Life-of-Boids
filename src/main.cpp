#include "agent.hpp"
#include "display.h"

bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

int main() {

    // Window initialization
    GLFWwindow* window;
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

    // Global loop
    while (!glfwWindowShouldClose(window)) {
        updateAgentWindow(window, agents, obstacles,triangles);
        addAgent(window, addBird, addPredator, agents, obstacles, triangles);
        updateWindow(window, triangles, trianglesObs, triangle_vertexArray, triangleObs_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location);
    }

    endWindow(window);
}
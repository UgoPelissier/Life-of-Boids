#include "main.h"
#include "agent.hpp"
#include "display.h"

bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

int main() {

    // Window initialization
    GLFWwindow* window;
    VertexArray triangle_vertexArray;
    Buffer triangle_buffer;
    ShaderProgram triangle_shaderProgram;
    GLint mvp_location;
    std::tie(window, triangle_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location) = initWindow();

    // Agents initialization
    std::vector<Agent> agents;
    std::vector<std::array<triangle::Vertex, 3>> triangles; // Array that will contain the birds, represented with triangles
    std::tie(agents,triangles) = initAgentWindow();

    // Global loop
    while (!glfwWindowShouldClose(window)) {
        updateAgentWindow(window, agents, triangles);
        addAgent(window, addBird, addPredator, agents, triangles);
        updateWindow(window, triangles, triangle_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location);
    }

    endWindow(window);
}
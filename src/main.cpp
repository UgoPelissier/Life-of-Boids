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
        glBegin(GL_TRIANGLE_FAN); // Draw A 6 vertex polygon
        glVertex3f(-0.5f, 1.0f, 0.0f); // Top Left
        glVertex3f(-1.0f, 0.0f, 0.0f); // Left
        glVertex3f(-0.5f, 1.0f, 0.0f); // Bottom Left
        glVertex3f(0.5f, 1.0f, 0.0f); // Top Right
        glVertex3f(1.0f, 0.0f, 0.0f); // Right
        glVertex3f(0.5f, 1.0f, 0.0f); // Bottom Right

        updateAgentWindow(window, agents, triangles);
        addAgent(window, addBird, addPredator, agents, triangles);
        updateWindow(window, triangles, triangle_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location);
    }

    endWindow(window);
}
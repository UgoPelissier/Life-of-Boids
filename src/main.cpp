#include "main.h"

// agent addition params
bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

int main() {

    // Window initialization
    GLFWwindow* window;

    VertexArray triangleObs_vertexArray = {}, trianglePred_vertexArray = {}, triangleBird_vertexArray = {}, triangleTree_vertexArray = {}, triangleFruit_vertexArray = {};
    Buffer triangle_buffer = {0};
    ShaderProgram triangle_shaderProgram = {0};
    GLint mvp_location;

    std::tie(window,
             triangleObs_vertexArray,
             trianglePred_vertexArray,
             triangleBird_vertexArray,
             triangleTree_vertexArray,
             triangleFruit_vertexArray,
             triangle_buffer,
             triangle_shaderProgram,
             mvp_location) = initWindow();

    // Agents initialization
    vars::agentWindowVars_t var = initAgentWindow();

    // Global loop
    int i = 0, j = 0;
    double total_fps = 0.f;
    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {

        updateAgentWindow(window, var);
        addAgent(window, var);
        updateWindow(window,
                var.trianglesObs, var.trianglesPredators, var.trianglesBirds, var.trianglesTree, var.trianglesFruit,
                triangleObs_vertexArray,trianglePred_vertexArray,triangleBird_vertexArray,triangleTree_vertexArray,triangleFruit_vertexArray,
                triangle_buffer,triangle_shaderProgram,mvp_location);
   
        i++;
        if (i == NUMBER_LOOP_FPS) {
            auto end = std::chrono::high_resolution_clock::now();
            display_fps(window, start, end, total_fps);
            i = 0;
            start = std::chrono::high_resolution_clock::now();
            j++;
        }

    }

    std::cout << "Avg fps: " << total_fps / j << std::endl;
    
    endWindow(window);
}

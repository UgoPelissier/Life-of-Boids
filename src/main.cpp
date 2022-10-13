#include "common.hpp"
#include "display/display.hpp"
#include "movement/law.hpp"

int main() {

    std::vector<Agent> agents = initialiaze_agents();

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1200, 1000, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set our call back functions for user interaction
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window); // Initialize GLEW
    gladLoadGL();
    glfwSwapInterval(1);

    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    float ratio = (float)width / (float)height;

    // Triangle
    // New
    ShaderProgram triangle_shaderProgram
        = ShaderProgram_new(triangle::vertex_shader_text, triangle::fragment_shader_text);
    VertexArray triangle_vertexArray = VertexArray_new();
    Buffer triangle_buffer = Buffer_new();
    // Init
    VertexArray_bind(triangle_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    // Define initial triangles
    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::mt19937_64 rng;

    float x, y;
    vec3 randomColor;
    std::vector<std::array<triangle::Vertex, 3>> triangles; // Array that will contain the birds, represented with triangles

    for (Agent agent : agents) {
        if (agent.get_type() == 1) {
            //randomColor = { float(unif(rng)), float(unif(rng)), float(unif(rng)) };
            randomColor = { 0, 100, 0 };
        }
        else {
            randomColor = { 100, 0, 0 };

        }
        

        x = 2 * ratio * (((float)(agent.get_x())) / (float)(WIDTH)) - ratio;
        y = 2 * (((float)(agent.get_y())) / (float)(HEIGHT)) - 1;

        triangles.push_back(triangle::newTriangle({ x,y }, randomColor, agent.get_angle()));
    }

    const GLint mvp_location = ShaderProgram_getUniformLocation(triangle_shaderProgram, "MVP");
    const GLint vpos_location = ShaderProgram_getAttribLocation(triangle_shaderProgram, "vPos");
    const GLint vcol_location = ShaderProgram_getAttribLocation(triangle_shaderProgram, "vCol");

    glVertexAttribPointer(
        vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, pos));
    glVertexAttribPointer(
        vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, col));
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);

    // Global loop
    std::cout << "To add a new agent: move the mouse to the desired location and press 'b'" << std::endl;

   while (!glfwWindowShouldClose(window)) {

        glfwGetFramebufferSize(window, &width, &height); // Get window size
        ratio = (float)width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        law_function(agents);
        
        for (size_t i(0); i < agents.size(); i++) {

            x = 2 * ratio * (((float)(agents[i].get_x())) / (float)(WIDTH)) - ratio;
            y = 2 * (((float)(agents[i].get_y())) / (float)(HEIGHT)) - 1;

            triangles[i] = triangle::newTriangle({ x,y }, triangles[i][0].col, agents[i].get_angle());
        }

        if (add) { // Add new triangle to the window
            randomColor = { float(unif(rng)), float(unif(rng)), float(unif(rng)) };

            std::cout << (int)xpos << " " << HEIGHT - (int)ypos << std::endl;

            agents.push_back(Agent((int)xpos, HEIGHT - (int)ypos, 2 * PI * unif(rng), unif(rng), 1, RANGE_COHESION_HIGH, RANGE_COHESION_LOW, RANGE_SEPARATION, RANGE_ALIGMENT_HIGH, RANGE_ALIGMENT_LOW, SPEED));

            triangles.push_back(triangle::newTriangle({ 2 * ratio * (((float)((float)xpos)) / (float)(WIDTH)) - ratio , 2 * (((float)(HEIGHT - (int)ypos)) / (float)(HEIGHT)) - 1 },
                randomColor,
                2 * PI * unif(rng)));
        }
        add = false;

        {  // Triangle
            mat4x4 m = triangle::mat4x4_identity();
            mat4x4 p = triangle::mat4x4_ortho(-ratio, ratio, -1., 1., 1., -1.); // Projection matrix (Visualization operation)
            mat4x4 mvp = triangle::mat4x4_mul(p, m); // MVP = Projection (* View) * Model (= Translation * Rotation)

            VertexArray_bind(triangle_vertexArray);
            Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
            ShaderProgram_activate(triangle_shaderProgram);

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
            glBindVertexArray(triangle_vertexArray.vertex_array);

            glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
            glBufferData(GL_ARRAY_BUFFER, 3 * triangles.size() * sizeof(triangle::Vertex), triangles.data(), GL_STREAM_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 3 * triangles.size());
        }

        glfwSetWindowTitle(window, "Life of boids"); // Set window title

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
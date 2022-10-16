#include "display.h"
#include "../movement/agent.hpp"
#include "glx.hpp"
#include "triangle.hpp"

void error_callback(int error, const char* description) {
    std::cerr << "Error[" << error << "]: " << description << "\n";
}

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        std::puts("Export current display");
        char export_filename[] = "export.png";
        saveImage(export_filename, window);
    }
    // Add a new bird by positioning the cursor at the desired location and pressing "b"
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        addBird = true;
    }
    // Add a new predator by positioning the cursor at the desired location and pressing "p"
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        addPredator = true;
    }
}

vec2 scale(Agent& agent) {
    return {
            2 * RATIO * (((Real)(agent.get_x())) / (Real)(WIDTH)) - RATIO,
            2 * (((Real)(agent.get_y())) / (Real)(HEIGHT)) - 1
    };
}

vec2 scale(Agent& agent, Real ratio) {
    return {
            2 * ratio * (((Real)(agent.get_x())) / (Real)(WIDTH)) - ratio,
            2 * (((Real)(agent.get_y())) / (Real)(HEIGHT)) - 1
    };
}

vec2 scale(Obstacle& obstacle) {
    return {
            2 * RATIO * (((Real)(obstacle.get_x())) / (Real)(WIDTH)) - RATIO,
            2 * (((Real)(obstacle.get_y())) / (Real)(HEIGHT)) - 1
    };
}

std::tuple<GLFWwindow*, VertexArray, VertexArray, Buffer, ShaderProgram, GLint> initWindow() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Triangle", nullptr, nullptr);
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

    // Triangle
    // New
    ShaderProgram triangle_shaderProgram
            = ShaderProgram_new(triangle::vertex_shader_text, triangle::fragment_shader_text);
    VertexArray triangle_vertexArray = VertexArray_new();

    const GLint mvp_location = ShaderProgram_getUniformLocation(triangle_shaderProgram, "MVP");
    const GLint vpos_location = ShaderProgram_getAttribLocation(triangle_shaderProgram, "vPos");
    const GLint vcol_location = ShaderProgram_getAttribLocation(triangle_shaderProgram, "vCol");

    // Agent triangle
    //New
    Buffer triangle_buffer = Buffer_new();
    // Init
    VertexArray_bind(triangle_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glVertexAttribPointer(
            vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, pos));
    glVertexAttribPointer(
            vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, col));
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);

    // Obstacle triangle
    //New
    VertexArray triangleObs_vertexArray = VertexArray_new();
    // Init
    VertexArray_bind(triangleObs_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glVertexAttribPointer(
            vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, pos));
    glVertexAttribPointer(
            vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, col));
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);

    return std::make_tuple(window, triangle_vertexArray, triangleObs_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location);
}

std::tuple<std::vector<Agent>, std::vector<Obstacle>, std::vector<std::array<triangle::Vertex, 3>>, std::vector<std::array<triangle::Vertex, 3>>> initAgentWindow() {

    std::cout << "To add a new agent: move the mouse to the desired location and press 'b' for a bird or 'p' for a predator" << std::endl;

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
    return std::make_tuple(agents, obstacles,triangles, trianglesObs);
}

void updateAgentWindow(GLFWwindow* window, std::vector<Agent>& agents, std::vector<Obstacle>& obstacles, std::vector<std::array<triangle::Vertex, 3>>& triangles) {
    triangles = {};

    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    agents = updateAgents(agents, obstacles);

    for (auto & agent : agents) {
        if (agent.get_predator()) {
            triangles.push_back(triangle::newTriangle(scale(agent, ratio), PRED_COLOR, agent.get_angle(), 2 * BODY_SIZE));
        }
        else {
            triangles.push_back(triangle::newTriangle(scale(agent, ratio), BIRD_COLOR, agent.get_angle(), BODY_SIZE));
        }
    }

}

void addAgent(GLFWwindow* window, bool& addBird, bool& addPredator, std::vector<Agent>& agents, std::vector<Obstacle>& obstacles, std::vector<std::array<triangle::Vertex, 3>>& triangles) {
    int width{}, height{};
    size_t n = agents.size();
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;
    Agent newAgent;
    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    if (addBird) { // Add new bird to the window
        newAgent = Agent(cursorX, HEIGHT - cursorY, 2 * PI * unif(engine) - PI, false,n);
        newAgent.obstacle(obstacles);
        if ( !newAgent.get_obstacle() && !newAgent.overlap(agents) ) {
            agents.push_back(newAgent);
            n = agents.size();
            triangles.push_back(triangle::newTriangle(
                    scale(newAgent, ratio),
                    BIRD_COLOR,
                    newAgent.get_angle(),
                    BODY_SIZE));
        }
        addBird = false;
    }

    if (addPredator) { // Add new predator to the window
        newAgent = Agent(cursorX, HEIGHT - cursorY, 2 * PI * unif(engine) - PI, true,n);
        newAgent.obstacle(obstacles);
        if ( !newAgent.get_obstacle() && !newAgent.overlap(agents) ) {
            agents.push_back(newAgent);
            n = agents.size();
            triangles.push_back(triangle::newTriangle(
                    scale(newAgent, ratio),
                    PRED_COLOR,
                    newAgent.get_angle(),
                    2 * BODY_SIZE));
        }
        addPredator = false;
    }
}

void updateWindow(GLFWwindow* window,
                  std::vector<std::array<triangle::Vertex, 3>>& triangles,
                  std::vector<std::array<triangle::Vertex, 3>>& trianglesObs,
                  VertexArray& triangle_vertexArray,
                  VertexArray& triangleObs_vertexArray,
                  Buffer& triangle_buffer,
                  ShaderProgram& triangle_shaderProgram,
                  GLint& mvp_location) {

    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;

    mat4x4 p = triangle::mat4x4_ortho(-ratio, ratio, -1., 1., 1., -1.); // Projection matrix (Visualization operation)

    VertexArray_bind(triangle_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(triangle_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 3 * triangles.size() * sizeof(triangle::Vertex), triangles.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3 * triangles.size());

    VertexArray_bind(triangleObs_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(triangleObs_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 3 * trianglesObs.size() * sizeof(triangle::Vertex), trianglesObs.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3 * trianglesObs.size());

    glfwSetWindowTitle(window, "Life of boids"); // Set window title

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void endWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
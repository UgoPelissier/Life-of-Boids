#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <random>

using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat2x2 = std::array<vec2, 2>;
using mat3x3 = std::array<vec3, 3>;
using mat4x4 = std::array<vec4, 4>;

#include "../common.hpp"
#include "glx/glx.hpp"
#include "shaders/lines.hpp"
#include "shaders/points.hpp"
#include "shaders/triangle.hpp"

static bool add(false);
static double xpos, ypos;

static vec2 scale(double xPos, double yPos, int Width, int Height, float Ratio) {
    float x(0), y(0);
    x = (float)(2*Ratio*(xPos/(Width/2))-Ratio);
    y = (float)(2*((-yPos)/(Height/2))+1);
    return {x,y};
}

static void error_callback(int error, const char* description) {
    std::cerr << "Error[" << error << "]: " << description << "\n";
}

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
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
        glfwGetCursorPos(window, &xpos, &ypos);
        add = true;
    }
}

int main() {
    // Get the initial number of birds
    std::cout << std::endl << "How many birds to start ? ";
    size_t initialNumberBirds(0);
    std::cin >> initialNumberBirds;
    std::cout << std::endl;

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

    std::vector<vec2> centers;
    std::vector<double> orientations;

    vec2 randomCenter;
    vec3 randomColor;
    double randomOrientation;
    std::vector<std::array<triangle::Vertex, 3>> triangles; // Array that will contain the birds, represented with triangles

    for (size_t i = 0; i<initialNumberBirds; ++i) {

        randomCenter = {(float)(ratio*((2*unif(rng))-1)), (float)((2*unif(rng))-1)};
        randomColor = {float(unif(rng)), float(unif(rng)), float(unif(rng))};
        randomOrientation = 2*PI*unif(rng);

        if ( (not overlap(randomCenter, centers)) and (not outsideWindow(randomCenter, ratio)) ) {
            centers.push_back(randomCenter);
            orientations.push_back(randomOrientation);
            triangles.push_back(triangle::newTriangle(randomCenter, randomColor, randomOrientation));
        }
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

    vec2 center;

    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height); // Get window size
        ratio = (float)width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        if (add) { // Add new triangle to the window
            center = scale(xpos, ypos, width, height, ratio);
            if ( (not overlap(center, centers)) and (not outsideWindow(center, ratio)) ) {
                centers.push_back(center);
                randomColor = {float(unif(rng)), float(unif(rng)), float(unif(rng))};
                randomOrientation = 2 * PI * unif(rng);
                orientations.push_back(randomOrientation);
                triangles.push_back(triangle::newTriangle(center,
                                                          randomColor,
                                                          randomOrientation));
            }
            add = false;
        }

        {  // Triangle
            mat4x4 m = triangle::mat4x4_identity();
            mat4x4 p = triangle::mat4x4_ortho(-ratio, ratio, -1., 1., 1., -1.); // Projection matrix (Visualization operation)
            mat4x4 mvp = triangle::mat4x4_mul(p, m); // MVP = Projection (* View) * Model (= Translation * Rotation)

/*            for (auto& triangle : triangles) {
                    triangle = triangle::rotate(triangle, angle, origin);
                triangle = triangle::translate(triangle, X, Y);
            }*/

            VertexArray_bind(triangle_vertexArray);
            Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
            ShaderProgram_activate(triangle_shaderProgram);

            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
            glBindVertexArray(triangle_vertexArray.vertex_array);

            glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
            glBufferData(GL_ARRAY_BUFFER, 3*triangles.size() * sizeof(triangle::Vertex), triangles.data(), GL_STREAM_DRAW);
            glDrawArrays(GL_TRIANGLES, 0, 3*triangles.size());
        }

        glfwSetWindowTitle(window, "Life of boids"); // Set window title

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
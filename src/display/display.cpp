#include "display.h"

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

std::vector<Fruit> updateObjects(std::vector<Obstacle>& obstacles,
                                      predators_t& predators,
                                      birds_t& birds,
                                      std::vector<Tree>& trees,
                                      std::vector<Fruit>& fruits) {

    std::vector<Fruit> newFruits;
    for (size_t i(0); i < predators.size(); i++) {
        predators[i].update(obstacles, predators, birds);
    }

    for (size_t i(0); i < birds.size(); i++) {
        bool is_alive = birds[i].update(obstacles, predators, birds, fruits);
        if (!is_alive)
            birds.erase(i);
    }

    for (Tree& tree : trees) {
        tree.DropFruitAndAppend(fruits, obstacles);
    }

    for (Fruit& fruit : fruits) {
        if (fruit.get_alive())
            newFruits.push_back(fruit);
    }

    return newFruits;
}


std::tuple<
        GLFWwindow*,
        VertexArray,
        VertexArray,
        VertexArray,
        VertexArray,
        VertexArray,
        Buffer,
        ShaderProgram,
        GLint> initWindow() {

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
    const GLint mvp_location = ShaderProgram_getUniformLocation(triangle_shaderProgram, "MVP");
    const GLint vpos_location = ShaderProgram_getAttribLocation(triangle_shaderProgram, "vPos");
    const GLint vcol_location = ShaderProgram_getAttribLocation(triangle_shaderProgram, "vCol");
    Buffer triangle_buffer = Buffer_new();

    // Birds triangle
    //New
    VertexArray triangleBird_vertexArray = VertexArray_new();
    // Init
    VertexArray_bind(triangleBird_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glVertexAttribPointer(
            vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, pos));
    glVertexAttribPointer(
            vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, col));
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);

    // Birds triangle
    //New
    VertexArray trianglePred_vertexArray = VertexArray_new();
    // Init
    VertexArray_bind(trianglePred_vertexArray);
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

    // Tree triangle
    //New
    VertexArray triangleTree_vertexArray = VertexArray_new();
    // Init
    VertexArray_bind(triangleTree_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glVertexAttribPointer(
            vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, pos));
    glVertexAttribPointer(
            vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, col));
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);


    // Fruit triangle
    //New
    VertexArray triangleFruit_vertexArray = VertexArray_new();
    // Init
    VertexArray_bind(triangleFruit_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glVertexAttribPointer(
            vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, pos));
    glVertexAttribPointer(
            vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(triangle::Vertex), (void*)offsetof(triangle::Vertex, col));
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(vcol_location);

    return std::make_tuple(window, triangleObs_vertexArray, trianglePred_vertexArray,triangleBird_vertexArray, triangleTree_vertexArray, triangleFruit_vertexArray, triangle_buffer, triangle_shaderProgram, mvp_location);
}

std::tuple<
        std::vector<Obstacle>,
        predators_t,
        birds_t,
        std::vector<Tree>,
        std::vector<Fruit>,
        triangle::vertices_t,
        triangle::vertices_t,
        triangle::vertices_t,
        triangle::vertices_t,
        triangle::vertices_t
>
initAgentWindow() {

    std::cout << "To add a new agent: move the mouse to the desired location and press 'b' for a bird or 'p' for a predator" << std::endl;

    std::vector<Obstacle> obstacles = Obstacle::init();
    predators_t predators = Predator::init(obstacles);
    birds_t birds = Bird::init(obstacles, predators);

    std::vector<Tree> trees = Tree::init(obstacles);
    std::vector<Fruit> fruits = {};
    for (Tree tree : trees) {
        tree.DropFruitAndAppend(fruits, obstacles);
    }

    triangle::vertices_t trianglesPredators(predators.size());
    triangle::vertices_t trianglesBirds(birds.size());

    triangle::vertices_t trianglesObs(obstacles.size());
    triangle::vertices_t obstacle(2);

    triangle::vertices_t trianglesTree(trees.size());
    triangle::vertices_t tree_triangles(2);
    triangle::vertices_t trianglesFruit;
    triangle::vertices_t fruit_triangles(2);

    for (auto& it : predators) {
        Predator& predator = it.second;
        trianglesPredators.push_back(triangle::newTriangle(Object::scale(predator), PRED_COLOR, predator.get_angle(), 2 * BODY_SIZE));
    }
    for (auto& it : birds) {
        Bird& bird = it.second;
        trianglesBirds.push_back(triangle::newTriangle(Object::scale(bird), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
    }
    for (Obstacle const& obs : obstacles) {
        obstacle = triangle::newObstacle(Object::scale(obs), OBSTACLE_COLOR, obs.get_height()/(Real)HEIGHT, obs.get_width()/(Real)WIDTH);
        for (const auto & i : obstacle) {
            trianglesObs.push_back(i);
        }
    }
    for (Tree const& tree : trees) {
        tree_triangles = triangle::newTree(Object::scale(tree), TREE_COLOR, tree.get_height() / (Real)HEIGHT, tree.get_width() / (Real)WIDTH);
        for (const auto & i : tree_triangles) {
            trianglesTree.push_back(i);
        }
    }
    for (Fruit const& fruit : fruits) {
        fruit_triangles = triangle::newFruit(Object::scale(fruit), FRUIT_COLOR, fruit.get_size()/(Real)WIDTH);
        for (const auto & i : fruit_triangles) {
            trianglesFruit.push_back(i);
        }
    }
    return std::make_tuple(obstacles, predators, birds,trees, fruits, trianglesObs, trianglesPredators, trianglesBirds, trianglesTree, trianglesFruit);
}

void updateAgentWindow(
        GLFWwindow* window,
        std::vector<Obstacle>& obstacles,
        predators_t& predators,
        birds_t& birds,
        std::vector<Tree>& trees,
        std::vector<Fruit>& fruits,
        triangle::vertices_t& trianglesPredators,
        triangle::vertices_t& trianglesBirds,
        triangle::vertices_t& trianglesFruit
        ) {

    trianglesPredators = {};
    trianglesBirds = {};
    trianglesFruit = {};
    triangle::vertices_t fruit_triangle;

    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    fruits = updateObjects(obstacles, predators, birds, trees, fruits);

    for (auto& it : predators) {
        Predator& predator = it.second;
        trianglesPredators.push_back(triangle::newTriangle(Object::scale(predator, ratio), PRED_COLOR, predator.get_angle(), 2 * BODY_SIZE));
    }

    for (auto& it : birds) {
        Bird& bird = it.second;
        trianglesBirds.push_back(triangle::newTriangle(Object::scale(bird, ratio), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
    }

    for (auto const& fruit : fruits) {
        fruit_triangle = triangle::newFruit(Object::scale(fruit), FRUIT_COLOR, fruit.get_size()/(Real)WIDTH);
        for (const auto & i : fruit_triangle) {
            trianglesFruit.push_back(i);
        }
    }

}

void addAgent(
        GLFWwindow* window,
        std::vector<Obstacle>& obstacles,
        predators_t& predators,
        birds_t& birds,
        triangle::vertices_t& trianglesPredators,
        triangle::vertices_t& trianglesBirds
        ) {
    int width{}, height{};

    size_t n_predators = predators.size();
    size_t n_birds = birds.size();

    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;

    Bird newBird;
    Predator newPredator;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    if (addBird) { // Add new bird to the window
        newBird = Bird((Real)cursorX, (Real)HEIGHT - (Real)cursorY, (Real)(2 * PI * unif(engine) - PI),n_birds);
        newBird.obstacle(obstacles);
        if ( newBird.get_state()!= state::near_obstacle && !newBird.overlap(birds, predators) ) {
            birds[n_birds++] = newBird;
            trianglesBirds.push_back(triangle::newTriangle(
                Object::scale(newBird, ratio),
                    BIRD_COLOR,
                    newBird.get_angle(),
                    BODY_SIZE));
        }
        addBird = false;
    }

    if (addPredator) { // Add new predator to the window
        newPredator = Predator((Real)cursorX, (Real)HEIGHT - (Real)cursorY, (Real)(2 * PI * unif(engine) - PI),n_predators);
        newPredator.obstacle(obstacles);
        if ( newPredator.get_state()!= state::near_obstacle && !newPredator.overlap(birds, predators)) {
            predators[n_predators++] = newPredator;
            trianglesPredators.push_back(triangle::newTriangle(
                Object::scale(newPredator, ratio),
                    PRED_COLOR,
                    newPredator.get_angle(),
                    2 * BODY_SIZE));
        }
        addPredator = false;
    }
}

void updateWindow(
        GLFWwindow* window,
        triangle::vertices_t& trianglesObs,
        triangle::vertices_t& trianglesPredators,
        triangle::vertices_t& trianglesBirds,
        triangle::vertices_t& trianglesTree,
        triangle::vertices_t& trianglesFruit,
        VertexArray& triangleObs_vertexArray,
        VertexArray& trianglePred_vertexArray,
        VertexArray& triangleBird_vertexArray,
        VertexArray& triangleTree_vertexArray,
        VertexArray& triangleFruit_vertexArray,
        Buffer& triangle_buffer,
        ShaderProgram& triangle_shaderProgram,
        GLint& mvp_location
        ) {

    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;

    mat4x4 p = triangle::mat4x4_ortho(-ratio, ratio, -1., 1., 1., -1.); // Projection matrix (Visualization operation)

    //Tree
    VertexArray_bind(triangleTree_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(triangleTree_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr (3 * trianglesTree.size() * sizeof(triangle::Vertex)), trianglesTree.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * trianglesTree.size()));

    //Fruit
    VertexArray_bind(triangleFruit_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(triangleFruit_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr (3 * trianglesFruit.size() * sizeof(triangle::Vertex)), trianglesFruit.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * trianglesFruit.size()));

    //Predators
    VertexArray_bind(trianglePred_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(trianglePred_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr (3 * trianglesPredators.size() * sizeof(triangle::Vertex)), trianglesPredators.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * trianglesPredators.size()));

    //Birds
    VertexArray_bind(triangleBird_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(triangleBird_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr (3 * trianglesBirds.size() * sizeof(triangle::Vertex)), trianglesBirds.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * trianglesBirds.size()));

    //Obstacles
    VertexArray_bind(triangleObs_vertexArray);
    Buffer_bind(triangle_buffer, GL_ARRAY_BUFFER);
    ShaderProgram_activate(triangle_shaderProgram);

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&p);
    glBindVertexArray(triangleObs_vertexArray.vertex_array);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr (3 * trianglesObs.size() * sizeof(triangle::Vertex)), trianglesObs.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(3 * trianglesObs.size()));

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void display_fps(GLFWwindow* window,
                std::chrono::time_point<std::chrono::high_resolution_clock>& start,
                std::chrono::time_point<std::chrono::high_resolution_clock>& end,
                double &total_fps) {

    std::chrono::duration<double, std::milli> float_ms = end - start;
    std::stringstream ss;
    double fps = (1 / (float_ms.count()) * 1000) * NUMBER_LOOP_FPS;
    ss << "FPS : " << fps;
    glfwSetWindowTitle(window, ss.str().c_str());
    total_fps += fps;

}

void endWindow(GLFWwindow* window) {

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
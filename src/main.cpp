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
    std::vector<Obstacle> obstacles;
    predators_t predators;
    birds_t birds;
    std::vector<Tree> trees;
    std::vector<Fruit> fruits;
    triangle::vertices_t trianglesObs;
    triangle::vertices_t trianglesPredators;
    triangle::vertices_t trianglesBirds;
    triangle::vertices_t trianglesTree;
    triangle::vertices_t trianglesFruit;

    std::tie(obstacles, predators, birds, trees, fruits,
             trianglesObs, trianglesPredators, trianglesBirds, trianglesTree, trianglesFruit) = initAgentWindow();

    // Global loop
    int i = 0, j = 0;
    double total_fps = 0.f;
    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {

        updateAgentWindow(window,
                obstacles,predators,birds,trees,fruits,
                trianglesPredators,trianglesBirds,trianglesFruit);

        addAgent(window,
                obstacles,predators,birds,
                trianglesPredators,trianglesBirds);
        updateWindow(window,
                trianglesObs,trianglesPredators,trianglesBirds,trianglesTree,trianglesFruit,
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
        obstacle = triangle::newObstacle(Object::scale(obs), OBSTACLE_COLOR, obs.get_height() / (Real)HEIGHT, obs.get_width() / (Real)WIDTH);
        for (const auto& i : obstacle) {
            trianglesObs.push_back(i);
        }
    }
    for (Tree const& tree : trees) {
        tree_triangles = triangle::newTree(Object::scale(tree), TREE_COLOR, tree.get_height() / (Real)HEIGHT, tree.get_width() / (Real)WIDTH);
        for (const auto& i : tree_triangles) {
            trianglesTree.push_back(i);
        }
    }
    for (Fruit const& fruit : fruits) {
        fruit_triangles = triangle::newFruit(Object::scale(fruit), FRUIT_COLOR, fruit.get_size() / (Real)WIDTH);
        for (const auto& i : fruit_triangles) {
            trianglesFruit.push_back(i);
        }
    }
    return std::make_tuple(obstacles, predators, birds, trees, fruits, trianglesObs, trianglesPredators, trianglesBirds, trianglesTree, trianglesFruit);
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
        fruit_triangle = triangle::newFruit(Object::scale(fruit), FRUIT_COLOR, fruit.get_size() / (Real)WIDTH);
        for (const auto& i : fruit_triangle) {
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
        newBird = Bird((Real)cursorX, (Real)HEIGHT - (Real)cursorY, (Real)(2 * PI * unif(engine) - PI), n_birds);
        newBird.closestObstacle(obstacles);
        if (newBird.get_state() != state::near_obstacle && !newBird.overlap(birds, predators)) {
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
        newPredator = Predator((Real)cursorX, (Real)HEIGHT - (Real)cursorY, (Real)(2 * PI * unif(engine) - PI), n_predators);
        newPredator.closestObstacle(obstacles);
        if (newPredator.get_state() != state::near_obstacle && !newPredator.overlap(birds, predators)) {
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

#include "main.h"
#include <algorithm>
#include <mutex>

static std::mutex kill_mtx;

#if defined _WIN32 // IF WINDOWS

#include <execution>
// COMMENT THE FOLLOWING DEFINE, IF YOU WANT TO RUN THE CODE SEQUENTIALLY
#define EXECUTION_PAR

#elif defined __APPLE__ //IF APPLE

// UNCOMMENT THE FOLLOWING DEFINE, IF YOU WANT TO RUN THE CODE SEQUENTIALLY
// #define APPLE_SEQ

std::vector<int> thread_index(int n) {
    int step = n/N_THREADS;
    if (n%N_THREADS!=0)
        step++;
    int count = 0;

    std::vector<int> index;
    while (count<n) {
        index.emplace_back(count);
        count += step;
    }
    index.emplace_back(n);
    return index;
}
#endif

std::vector<Fruit> updateObjects(std::vector<Obstacle>& obstacles,
                                    predators_t& predators,
                                    birds_t& birds,
                                    std::vector<Tree>& trees,
                                    std::vector<Fruit>& fruits) {

    std::vector<Fruit> newFruits;
    std::vector<size_t> kill;

/*
IF YOU FEEL LIKE THE FOLLOWING CODE IS CONFUSING.
VISUAL STUDIO WILL MAKE THE CODE BLACK AND WHITE IF IT IS NOT GONNA RUN BASED ON THE DIRECTIVES
*/

// RUN PARALLELLY ON WINDOWS WITH C++ PARALLELISM
#ifdef EXECUTION_PAR //IF 1
    std::for_each(std::execution::par_unseq,
        predators.begin(),
        predators.end(),
        [&](auto& it) {
            it.second.update(obstacles, predators, birds);
        });

    std::for_each(std::execution::par_unseq,
        birds.begin(),
        birds.end(),
        [&](auto& it) {
            bool is_alive = it.second.update(obstacles, predators, birds, fruits);

            // killing later cause some other thread might be reading that object.
            if (!is_alive) {
                kill_mtx.lock();
                kill.push_back(it.first);
                kill_mtx.unlock();
            }
        });

#else // IF 1 ELSE

    // following loop is sequential cause not enough workload
    for (auto& it : predators) {
        it.second.update(obstacles, predators, birds);
    }

//============================================BIRD-LOOP==============================================
// RUN PARALLELY ON APPLE WITH C++ THREADS.
#if defined(__APPLE__) && !defined(APPLE_SEQ) // IF 2
    std::vector<std::thread> birds_threads(N_THREADS);
    std::vector<int> index = thread_index((int)birds.size());

    for (int i = 0; i<(int)index.size()-1 ; ++i) {
        birds_threads[i] = std::thread([&birds, obstacles, &predators, &fruits, index, i, &kill]() { Bird::thread_update(birds,obstacles,predators,fruits,index[i],index[i+1],kill); });
    }

    for (auto& th : birds_threads)
        th.join();

// RUN SEQUENTIALLY
#else // IF 2 ELSE
    for (auto it = birds.begin(); it != birds.end();) {
        
        bool is_alive = it->second.update(obstacles, predators, birds, fruits);
        if (!is_alive)
            it = birds.erase(it);
        else
            it++;
    }
#endif // END IF 2 ELSE
//============================================BIRD-LOOP==============================================
#endif // END IF 1 ELSE

    // remove the dead agents from the list
    for (size_t& k : kill)
        birds.erase(k);
    // following loop is sequential cause not enough workload
    for (Tree& tree : trees) {
        tree.DropFruitAndAppend(fruits);
    }
    // following loop is sequential cause not enough workload
    for (Fruit& fruit : fruits) {
        if (fruit.get_alive())
            newFruits.push_back(fruit);
        else {
            size_t n = birds.size();
            birds[n] = Bird(fruit.get_x(), fruit.get_y(), 0, n);
        }
    }

    return newFruits;
}

vars::agentWindowVars_t initAgentWindow() {

    std::cout << "\nTo add a new agent: move the mouse to the desired location and press 'b' for a bird or 'p' for a predator\n" << std::endl;    
    vars::agentWindowVars_t var;
    var.obstacles = Obstacle::init();
    var.predators = Predator::init(var.obstacles);
    var.birds = Bird::init(var.obstacles, var.predators);

    var.trees = Tree::init(var.obstacles);
    var.fruits = {};
    for (Tree tree : var.trees) {
        tree.DropFruitAndAppend(var.fruits);
    }

    triangle::vertices_t obstacle(2), tree_triangles(2), fruit_triangles(2);

    for (auto& it : var.predators) {
        Predator& predator = it.second;
        var.trianglesPredators.push_back(triangle::newTriangle(Object::scale(predator), PRED_COLOR, predator.get_angle(), 2 * BODY_SIZE));
    }
    for (auto& it : var.birds) {
        Bird& bird = it.second;
        var.trianglesBirds.push_back(triangle::newTriangle(Object::scale(bird), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
    }
    for (Obstacle const& obs : var.obstacles) {
        obstacle = triangle::newObstacle(Object::scale(obs), OBSTACLE_COLOR, obs.get_height() / (Real)HEIGHT, obs.get_width() / (Real)WIDTH);
        for (const auto& i : obstacle) {
            var.trianglesObs.push_back(i);
        }
    }
    for (Tree const& tree : var.trees) {
        tree_triangles = triangle::newTree(Object::scale(tree), TREE_COLOR, tree.get_size() / (Real)HEIGHT);
        for (const auto& i : tree_triangles) {
            var.trianglesTree.push_back(i);
        }
    }
    for (Fruit const& fruit : var.fruits) {
        fruit_triangles = triangle::newFruit(Object::scale(fruit), FRUIT_COLOR, fruit.get_size() / (Real)WIDTH);
        for (const auto& i : fruit_triangles) {
            var.trianglesFruit.push_back(i);
        }
    }

    return var;
}

void updateAgentWindow(GLFWwindow* window, vars::agentWindowVars_t& var) {

    var.trianglesPredators = {};
    var.trianglesBirds = {};
    var.trianglesFruit = {};
    triangle::vertices_t fruit_triangle;

    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height); // Get window size
    Real ratio = (Real)width / (Real)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    var.fruits = updateObjects(var.obstacles, var.predators, var.birds, var.trees, var.fruits);

    for (auto& it : var.predators) {
        Predator& predator = it.second;
        var.trianglesPredators.push_back(triangle::newTriangle(Object::scale(predator, ratio), PRED_COLOR, predator.get_angle(), 2 * BODY_SIZE));
    }

    for (auto& it : var.birds) {
        Bird& bird = it.second;
        var.trianglesBirds.push_back(triangle::newTriangle(Object::scale(bird, ratio), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
    }

    for (auto const& fruit : var.fruits) {
        fruit_triangle = triangle::newFruit(Object::scale(fruit), FRUIT_COLOR, fruit.get_size() / (Real)WIDTH);
        for (const auto& i : fruit_triangle) {
            var.trianglesFruit.push_back(i);
        }
    }

    // Bird::clearIgnore(birds);
}

void addAgent(GLFWwindow* window, vars::agentWindowVars_t& var) {
    int width{}, height{};

    size_t n_predators = var.predators.size();
    size_t n_birds = var.birds.size();
    if (window != NULL)
        glfwGetFramebufferSize(window, &width, &height); // Get window size
    else {
        width = WIDTH;
        height = HEIGHT;
    }
    Real ratio = (Real)width / (Real)height;

    Bird newBird;
    Predator newPredator;

    std::uniform_real_distribution<Real> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    if (addBird) { // Add new bird to the window
        newBird = Bird((Real)cursorX, (Real)HEIGHT - (Real)cursorY, (Real)(2 * PI * unif(engine) - PI), n_birds);
        newBird.closestObstacle(var.obstacles);
        if (newBird.get_state() != state::near_obstacle && !newBird.overlap(var.birds, var.predators)) {
            var.birds[n_birds++] = newBird;
            var.trianglesBirds.push_back(triangle::newTriangle(
                Object::scale(newBird, ratio),
                BIRD_COLOR,
                newBird.get_angle(),
                BODY_SIZE));
        }
        addBird = false;
    }

    if (addPredator) { // Add new predator to the window
        newPredator = Predator((Real)cursorX, (Real)HEIGHT - (Real)cursorY, (Real)(2 * PI * unif(engine) - PI), n_predators);
        newPredator.closestObstacle(var.obstacles);
        if (newPredator.get_state() != state::near_obstacle && !newPredator.overlap(var.birds, var.predators)) {
            var.predators[n_predators++] = newPredator;
            var.trianglesPredators.push_back(triangle::newTriangle(
                Object::scale(newPredator, ratio),
                PRED_COLOR,
                newPredator.get_angle(),
                2 * BODY_SIZE));
        }
        addPredator = false;
    }
}

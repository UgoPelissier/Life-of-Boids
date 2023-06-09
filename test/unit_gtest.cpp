#include <iostream>
#include <execution>
#include <algorithm>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"

#include "main.h"
#include "object.h"
#include "obstacle.h"
#include "agent.h"
#include "predator.h"
#include "bird.h"
#include "eco.h"
#include "fruit.h"
#include "tree.h"
#include "display.h"

#if defined _WIN32 || defined __linux__ // IF WINDOWS or LINUX
#include <execution>
#define EXECUTION_PAR
#endif

bool addBird = false;
bool addPredator = false;
double cursorX = 0;
double cursorY = 0;

// blank namespace to avoid implementation collision
namespace {

TEST(FirstGTest, ExpectCanary) {
  EXPECT_EQ(1, 1) << "Canary could not fail!";
}

TEST(Initialization, VectorSize) {

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

    EXPECT_EQ(var.obstacles.size(), DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";
    EXPECT_EQ(var.trianglesObs.size(), 2*DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";

    EXPECT_EQ(var.predators.size(), DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";
    EXPECT_EQ(var.trianglesPredators.size(), DEFAULT_NUM_PREDATORS) << "Not the correct number of obstacles!";

    EXPECT_EQ(var.birds.size(), DEFAULT_NUM_BIRDS) << "Not the correct number of agents!";
    EXPECT_EQ(var.trianglesBirds.size(), DEFAULT_NUM_BIRDS) << "Not the correct number of obstacles!";

    EXPECT_EQ(var.trees.size(), DEFAULT_NUM_TREES) << "Not the correct number of obstacles!";
    EXPECT_EQ(var.trianglesTree.size(), 6*DEFAULT_NUM_TREES) << "Not the correct number of obstacles!";
}

TEST(Initialization, Position) {

    vars::agentWindowVars_t var;
    var.obstacles = Obstacle::init();
    var.predators = Predator::init(var.obstacles);
    var.birds = Bird::init(var.obstacles, var.predators);

    int N = (int)DEFAULT_NUM_BIRDS;
    std::uniform_int_distribution uni(0, N); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());
    int i = uni(engine);

    for (size_t j(0); j<DEFAULT_NUM_BIRDS; j++) {
        var.trianglesBirds.push_back(triangle::newTriangle(Object::scale(var.birds[j]), BIRD_COLOR, var.birds[j].get_angle(), BODY_SIZE));
    }

    vec2 center = Object::scale(var.birds[i]);

    EXPECT_NEAR(center[0], triangle::center(var.trianglesBirds[i])[0],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    EXPECT_NEAR(center[1], triangle::center(var.trianglesBirds[i])[1],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    
    bool ObstacleBorder = false;

    for (size_t j(0); j < DEFAULT_NUM_OBSTACLES; j++) {
        if (var.obstacles[j].nearBorder()) {
            ObstacleBorder = true;
        }
    }

    EXPECT_FALSE(ObstacleBorder) << "An obstacle is near a border";
}

TEST(Function, FieldView) {
    Bird behind(-10,0,0);

    std::vector<Real> angles = {0,PI/6,PI/4,PI/3};
    std::vector<bool> inside_field_of_view;

    for (Real const& angle : angles) {
        Bird b(0,0,angle);
        inside_field_of_view.emplace_back(b.insideFieldView(behind));
    }

    std::vector<bool> answer = {false,false,true,true};

    for (size_t i = 0; i < answer.size(); ++i) {
        EXPECT_TRUE(answer[i]==inside_field_of_view[i]) << "Incorrect detection of the field of view of an agent";
    }
}

TEST(Function, Overlap) {
    birds_t birds;
    predators_t predators;
    size_t index(0);
    birds[index] = Bird(0, 0, 0, index);
    predators[index] = Predator(0, 0, 0, index);

    Bird bird_1 = Bird(0, 0, 0, index);

    Bird bird_2 = Bird(HEIGHT, WIDTH, 0, index);

    EXPECT_TRUE(bird_1.overlap(birds, predators));
    EXPECT_FALSE(bird_2.overlap(birds, predators));

    std::vector<Obstacle> obstacles = { Obstacle(0, 0, 50, 50) };

    Obstacle newObstacle_1 = Obstacle(0, 0, 50, 50);
    Obstacle newObstacle_2 = Obstacle(HEIGHT, WIDTH, 50, 50);

    EXPECT_TRUE(newObstacle_1.overlap(obstacles)) << "Overlap law of obstacle has an error. Should detect an overlap.";
    EXPECT_FALSE(newObstacle_2.overlap(obstacles)) << "Overlap law of obstacle has an error. Shouldn't detect an overlap.";
}

TEST(Law, Constant) {
    birds_t birds;

    size_t index(0);
    birds[index] = Bird (0,0,0,index);

    index++;
    birds[index] = Bird(0,COHESION_RANGE+1,0,index);

    birds[0].neighbours(birds);

    EXPECT_TRUE(birds[0].get_state()==state::constant);
}

TEST(Law, Cohesion) {
    birds_t birds;

    size_t index(0);
    birds[index] = Bird(0,0,0,index);

    index ++;
    birds[index] = Bird(0,COHESION_RANGE-1,0, index);

    index ++;
    birds[index] = Bird(COHESION_RANGE-1,0,0, index);

    std::vector<Real> target = birds[0].neighbours(birds);

    EXPECT_TRUE(birds[0].get_state()==state::cohesion);
    EXPECT_NEAR(target[0],(COHESION_RANGE-1)/3,1e-10);
    EXPECT_NEAR(target[1],(COHESION_RANGE-1)/3,1e-10);
}

TEST(Law, Alignment) {
    birds_t birds;

    size_t index(0);
    birds[index] = Bird(0,0,0,index);

    index ++;
    birds[index] = Bird(0,ALIGNMENT_RANGE-1,PI/2, index);

    index ++;
    birds[index] = Bird((ALIGNMENT_RANGE-1)*cos(PI/6),(ALIGNMENT_RANGE-1)*sin(PI/6),PI/2+PI/12, index);

    index ++;
    birds[index] = Bird((ALIGNMENT_RANGE-1)*cos(PI/6),-(ALIGNMENT_RANGE-1)*sin(PI/6),PI/2-PI/12, index);

    std::vector<Real> target = birds[0].neighbours(birds);

    Real avg_angle = (3*PI/2)/4;

    EXPECT_TRUE(birds[0].get_state()==state::alignment);
    EXPECT_NEAR(target[0],avg_angle,1e-10);
}

TEST(Law, Separation) {

    birds_t birds;
    predators_t predators;

    size_t index(0);
    birds[index] = Bird(0,0,0,index);
    predators[index] = Predator(WIDTH/2, HEIGHT/2, 0, index);

    index ++;
    birds[index] = Bird(SEPARATION_RANGE-1,0,-PI, index);
    predators[index] = Predator(WIDTH/2 + SEPARATION_RANGE-1, HEIGHT/2, -PI, index);

    std::vector<Real> target1 = birds[0].neighbours(birds);
    std::vector<Real> target2 = predators[0].neighbours(birds, predators);
    
    
    EXPECT_TRUE(birds[0].get_state()==state::separation);
    EXPECT_TRUE(predators[0].get_state() == state::separation);

    EXPECT_NEAR(target1[0],birds[1].get_x(),1e-10);
    EXPECT_NEAR(target1[1],birds[1].get_y(),1e-10);

    EXPECT_NEAR(target2[0], predators[1].get_x(), 1e-10);
    EXPECT_NEAR(target2[1], predators[1].get_y(), 1e-10);
}

TEST(Law, Predator) {
    birds_t birds; predators_t predators;

    size_t index(0);

    predators[index] = Predator(0,PREDATOR_RANGE+1,-PI/2,index);

    birds[index] = Bird(0,0,0,index);
    birds[index].closestPredator(predators);

    EXPECT_TRUE(birds[index].get_state()==state::constant && predators[index].get_state()==state::constant);

    predators[index] = Predator(0,PREDATOR_RANGE-1,-PI/2,index);

    birds[index].closestPredator(predators);
    std::vector<Real> target = predators[index].neighbours(birds,predators);

    EXPECT_TRUE(birds[index].get_state()==state::near_predator && predators[index].get_state()==state::near_prey);
    EXPECT_NEAR(target[0],birds[index].get_x(),1e-10);
    EXPECT_NEAR(target[1],birds[index].get_y(),1e-10);

    index ++;
    birds[index] = Bird(0,SEPARATION_RANGE-1,0,index);

    birds[index].closestPredator(predators);
    EXPECT_TRUE(birds[index].get_state()==state::near_predator);
    birds[index].neighbours(birds);
    EXPECT_TRUE(birds[index].get_state()==state::near_predatorANDseparation);
}

TEST(Law, Fruit) {
    std::vector<Fruit> fruits = { Fruit(0,0) };

    size_t index(0);

    birds_t birds;
    birds[index] = Bird(-(FRUIT_RANGE+1),0,0,index);
    birds[index].closestFruit(fruits);
    EXPECT_TRUE(birds[index].get_state()==state::constant && fruits[0].get_alive()==true);

    birds[index] = Bird(-FRUIT_RANGE+1,0,0,index);
    birds[index].closestFruit(fruits);
    EXPECT_TRUE(birds[index].get_state()==state::near_fruit && fruits[0].get_alive()==true);

    birds[index] = Bird(-DEAD_RANGE+1,0,0,index);
    birds[index].closestFruit(fruits);
    EXPECT_TRUE(birds[index].get_state()==state::near_fruit && fruits[0].get_alive()==false);
}

TEST(Law, Obstacle) {
    int ObstacleHeight = 50;
    int ObstacleWidth = 20;

    std::vector<Obstacle> obstacles = { Obstacle(0,0,ObstacleHeight,ObstacleWidth) };

    size_t index(0);

    birds_t birds;
    birds[index] = Bird(ObstacleHeight/2-1, 0, 0, index);
    birds[index].closestObstacle(obstacles);
    EXPECT_TRUE(birds[index].get_state() == state::near_obstacle) << "Incorrect detection of the obstacle on x positive axis";

    birds[index] = Bird(-(ObstacleHeight / 2 - 1), 0, 0, index);
    birds[index].closestObstacle(obstacles);
    EXPECT_TRUE(birds[index].get_state() == state::near_obstacle) << "Incorrect detection of the obstacle on x negative axis";

    birds[index] = Bird(0, ObstacleWidth / 2 - 1, 0, index);
    birds[index].closestObstacle(obstacles);
    EXPECT_TRUE(birds[index].get_state() == state::near_obstacle) << "Incorrect detection of the obstacle on y positive axis";

    birds[index] = Bird(0, -(ObstacleWidth / 2 - 1), 0, index);
    birds[index].closestObstacle(obstacles);
    EXPECT_TRUE(birds[index].get_state() == state::near_obstacle) << "Incorrect detection of the obstacle on y negative axis";
}

TEST(Feature, Tree) {

    std::vector<Obstacle> obstacles = Obstacle::init();
    Tree tree(HEIGHT/2, 10, 10, 0);
    std::vector<Fruit> fruits;
    Object obj(WIDTH / 2 + MAX_FRUIT_DISTANCE, HEIGHT / 2 + MAX_FRUIT_DISTANCE);
    Real max_dist = tree.distance(obj);

    tree.DropFruitAndAppend(fruits);
    if (fruits.size() == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(FRUIT_TIME_MAX));
        tree.DropFruitAndAppend(fruits);
    }
    EXPECT_GT(fruits.size(), 0);
    EXPECT_LE(fruits.size(), DEFAULT_NUM_FRUITS_DROPS);
    for (Fruit& f : fruits) {
        EXPECT_LE(tree.distance(f), max_dist);
    }
}
TEST(Feature, spawnBirdAfterFruitDies) {
    
    size_t index = 0;
    birds_t birds;
    std::vector<Fruit> fruits = { Fruit(0,0) };

    birds[index] = Bird(-DEAD_RANGE + 1, 0, 0, index);
    size_t old_c = birds.size();
    birds[index].closestFruit(fruits);
    
    index++;
    birds[index] = Bird(fruits[0].get_x(), fruits[0].get_y(), 0, index);
    size_t new_c = birds.size();

    EXPECT_GT(new_c, old_c);
    EXPECT_TRUE(fruits[0].get_alive() == false && 
            birds[index].get_x() == fruits[0].get_x() &&
            birds[index].get_y() == fruits[0].get_y());
}

TEST(Feature, addAgents) {

    vars::agentWindowVars_t var = initAgentWindow();
    
    size_t before_b = var.birds.size();
    size_t before_p = var.predators.size();
    addBird = false;
    addPredator = false;
    addAgent(NULL, var);
    size_t after_b = var.birds.size();
    size_t after_p = var.predators.size();
    EXPECT_EQ(after_b, before_b);
    EXPECT_EQ(after_p, before_p);
    
    before_b = var.birds.size();
    before_p = var.predators.size();

    for (int i = 0; i < 5; i++) {
        cursorX = (i * 100.);
        cursorY = (i * 100.);
        addBird = true;
        addAgent(NULL, var);
        addPredator = true;
        cursorX = 60 + (i * 100.);
        cursorY = 60 + (i * 100.);
        addAgent(NULL, var);
    }
    after_b = var.birds.size();
    after_p = var.predators.size();

    EXPECT_GT(after_b, before_b);
    EXPECT_GT(after_p, before_p);
}

#ifdef EXECUTION_PAR

TEST(Feature, parallelisation) {

    size_t n = 10000;
    float val = 3.1412;
    std::vector<float> a(n);

    std::for_each(std::execution::par_unseq,
        a.begin(), a.end(),
        [n, val](float& x) {
            x = val * val;
        }
    );
    // not really interested in the precision
    int v1 = (int)std::accumulate(a.begin(), a.end(), 0.0f);
    int v2 = (int)n * val * val;

    EXPECT_EQ(v1, v2);
}

#endif
}  // namespace
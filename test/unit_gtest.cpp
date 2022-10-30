#include <iostream>
#include "gtest/gtest.h"
#include "display.h"

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

    std::vector<Obstacle> obstacles = obstacles_init();
    std::vector<Agent> predators = predators_init(obstacles);
    std::vector<Bird> birds = birds_init(obstacles, predators);
    std::vector<Tree> trees = trees_init(obstacles);

    std::vector<std::array<triangle::Vertex, 3>> trianglesPredators(predators.size());
    std::vector<std::array<triangle::Vertex, 3>> trianglesBirds(birds.size());

    std::vector<std::array<triangle::Vertex, 3>> trianglesObs;
    std::vector<std::array<triangle::Vertex, 3>> obstacle;

    std::vector<std::array<triangle::Vertex, 3>> trianglesTree;
    std::vector<std::array<triangle::Vertex, 3>> tree_triangles;

    for (Agent const& predator : predators) {
        trianglesPredators.push_back(triangle::newTriangle(scale(predator), PRED_COLOR, predator.get_angle(), 2 * BODY_SIZE));
        }
    for (Bird const& bird : birds) {
        trianglesBirds.push_back(triangle::newTriangle(scale(bird), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
        }
    for (Obstacle const& obs : obstacles) {
        obstacle = triangle::newObstacle(scale(obs), OBSTACLE_COLOR, obs.get_height()/(Real)HEIGHT, obs.get_width()/(Real)WIDTH);
        for (const auto & i : obstacle) {
            trianglesObs.push_back(i);
            }
        }
    for (Tree const& tree : trees) {
        tree_triangles = triangle::newTree(scale(tree), TREE_COLOR, tree.get_height() / (Real)HEIGHT, tree.get_width() / (Real)WIDTH);
        for (const auto & i : tree_triangles) {
                trianglesTree.push_back(i);
            }
    }

    EXPECT_EQ(obstacles.size(), DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";
    EXPECT_EQ(trianglesObs.size(), 2*DEFAULT_NUM_OBSTACLES) << "Not the correct number of obstacles!";

    EXPECT_EQ(predators.size(), DEFAULT_NUM_PREDATORS) << "Not the correct number of agents!";
    EXPECT_EQ(trianglesPredators.size(), DEFAULT_NUM_PREDATORS) << "Not the correct number of obstacles!";

    EXPECT_EQ(birds.size(), DEFAULT_NUM_BIRDS) << "Not the correct number of agents!";
    EXPECT_EQ(trianglesBirds.size(), DEFAULT_NUM_BIRDS) << "Not the correct number of obstacles!";

    EXPECT_EQ(trees.size(), DEFAULT_NUM_TREES) << "Not the correct number of obstacles!";
    EXPECT_EQ(trianglesTree.size(), 2*DEFAULT_NUM_TREES) << "Not the correct number of obstacles!";
}

TEST(Initialization, Position) {

    std::vector<Obstacle> obstacles = obstacles_init();
    std::vector<Agent> predators = predators_init(obstacles);
    std::vector<Bird> birds = birds_init(obstacles, predators);

    std::vector<std::array<triangle::Vertex, 3>> trianglesBirds(birds.size());

    for (Bird const& bird : birds) {
        trianglesBirds.push_back(triangle::newTriangle(scale(bird), BIRD_COLOR, bird.get_angle(), BODY_SIZE));
    }

    int N = (int)DEFAULT_NUM_BIRDS;
    std::uniform_int_distribution uni(0, N); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::random_device dev;
    std::mt19937 engine(dev());

    int i = uni(engine);

    vec2 center = scale(birds[i]);

    EXPECT_NEAR(center[0], triangle::center(trianglesBirds[i])[0],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
    EXPECT_NEAR(center[1], triangle::center(trianglesBirds[i])[1],1e-6) << "Agent is not well located from the pixel grid [0:WIDTH]*[0:HEIGHT] to the OpenGL grid [-1:1]*[-1:1]";
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

TEST(Law, Constant) {
     size_t index(0);
     Bird b1(0,0,0,index);

     index++;
     Bird b2(0,COHESION_RANGE+1,0,index);

     b1.neighbours({b2});

     EXPECT_TRUE(b1.get_state()==constant);
}

TEST(Law, Cohesion) {
    size_t index(0);
    Bird b1(0,0,0,index);

    index ++;
    Bird b2(0,COHESION_RANGE-1,0, index);

    index ++;
    Bird b3(COHESION_RANGE-1,0,0, index);

    std::vector<Real> target = b1.neighbours({b1,b2,b3});

    EXPECT_TRUE(b1.get_state()==cohesion);
    EXPECT_NEAR(target[0],(COHESION_RANGE-1)/3,1e-10);
    EXPECT_NEAR(target[1],(COHESION_RANGE-1)/3,1e-10);
}

TEST(Law, Alignment) {
    size_t index(0);
    Bird b1(0,0,0,index);

    index ++;
    Bird b2(0,ALIGNMENT_RANGE-1,PI/2, index);

    index ++;
    Bird b3((ALIGNMENT_RANGE-1)*cos(PI/6),(ALIGNMENT_RANGE-1)*sin(PI/6),PI/2+PI/12, index);

    index ++;
    Bird b4((ALIGNMENT_RANGE-1)*cos(PI/6),-(ALIGNMENT_RANGE-1)*sin(PI/6),PI/2-PI/12, index);

    std::vector<Real> target = b1.neighbours({b1,b2,b3,b4});

    Real avg_angle = (3*PI/2)/4;

    EXPECT_TRUE(b1.get_state()==alignment);
    EXPECT_NEAR(target[0],avg_angle,1e-10);
}

TEST(Law, Separation) {
    size_t index(0);
    Bird b1(0,0,0,index);

    index ++;
    Bird b2(SEPARATION_RANGE-1,0,-PI, index);

    std::vector<Real> target = b1.neighbours({b1,b2});

    EXPECT_TRUE(b1.get_state()==separation);
    EXPECT_NEAR(target[0],b2.get_x(),1e-10);
    EXPECT_NEAR(target[1],b2.get_y(),1e-10);
}

TEST(Law, Predator) {
    size_t index(0);
    Bird b1(0,0,0,index);
    Agent pred(0,PREDATOR_RANGE+1,-PI/2,index);

    b1.pred({pred});

    EXPECT_TRUE(b1.get_state()==constant && pred.get_state()==constant);

    pred = Agent(0,PREDATOR_RANGE-1,-PI/2,index);
    b1.pred({pred});
    std::vector<Real> target = pred.neighbour({}, birds2agents({b1}));

    EXPECT_TRUE(b1.get_state()==predator && pred.get_state()==predator);
    EXPECT_NEAR(target[0],b1.get_x(),1e-10);
    EXPECT_NEAR(target[1],b1.get_y(),1e-10);

    index ++;
    Bird b2(0,SEPARATION_RANGE-1,0,index);

    b2.pred({pred});
    EXPECT_TRUE(b2.get_state()==predator);
    b2.neighbours({b1,b2});
    EXPECT_TRUE(b2.get_state()==predatorANDseparation);
}

}  // namespace
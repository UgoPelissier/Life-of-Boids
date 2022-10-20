#include "config.h"

const size_t DEFAULT_NUM_AGENTS = 500;
const size_t DEFAULT_NUM_PREDATORS = 10;
const size_t DEFAULT_NUM_OBSTACLES = 10;
const size_t DEFAULT_NUM_FRUIT_TREES = 10;
const int DEFAULT_NUM_FRUITS_DROPS = 5;
const Real CLOSE = 50;
const Real SPEED = 3;
const Real PRED_SPEED = 3*SPEED/2;
const Real PI  = 3.14159265358979323846;
const Real VIEW_RANGE = 3*PI/2;

// Display parameters
const int WIDTH = 2400;
const int HEIGHT = 1300;
const int MAX_OBSTACLE_SIZE = HEIGHT/5;
const int MAX_FRUIT_TREE_SIZE = 75;
const int MIN_FRUIT_TREE_SIZE = 50;
const int MAX_FRUIT_DISTANCE = 100;


const Real  RATIO = (float)WIDTH / (float)HEIGHT;
const Real  BODY_SIZE = 0.015;
const Real BODY_OVERLAP = (sqrt(3)*BODY_SIZE/2)*(WIDTH/2);
const vec3 PRED_COLOR = { 1., 0., 0. };
const vec3 BIRD_COLOR = { 1., 1., 1. };
const vec3 OBSTACLE_COLOR = { 0.2, 0.2, 0.2 };
const vec3 TREE_COLOR = { (Real)98/(Real)256, (Real)52/(Real)256, (Real)18/(Real)256 };
const vec3 FRUIT_COLOR = {0., 1., 0.};
const int NUMBER_LOOP_FPS = 10;

// Laws parameters
const Real SEPARATION_RANGE = 2*sqrt(5)*BODY_SIZE*WIDTH/9;
const Real DEAD_RANGE = SEPARATION_RANGE/4;
const Real ALIGNMENT_RANGE = 3*SEPARATION_RANGE;
const Real COHESION_RANGE = 2*ALIGNMENT_RANGE;
const Real PREDATOR_RANGE = COHESION_RANGE;
const Real FRUIT_RANGE = COHESION_RANGE;

const Real SEPARATION_RELAXATION = 0.75;
const Real ALIGNMENT_RELAXATION = 0.9;
const Real COHESION_RELAXATION = 0.9;
const Real PREDATOR_RELAXATION = 0.9;
const Real OBSTACLE_RELAXATION = 0.5;

const int FRUIT_TIME_MAX = 25;
const int FRUIT_TIME_MIN = 0;

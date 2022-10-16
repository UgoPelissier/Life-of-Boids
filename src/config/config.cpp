#include "config.h"

const size_t DEFAULT_NUM_AGENTS = 500;
const size_t DEFAULT_NUM_PREDATORS = 5;
const size_t DEFAULT_NUM_OBSTACLES = 10;
const Real CLOSE = 50;
const Real SPEED = 1;
const Real PRED_SPEED = 3*SPEED/2;
const Real PI  = 3.14159265358979323846;
const Real VIEW_RANGE = 3*PI/2;

// Display parameters
const int WIDTH = 2400;
const int HEIGHT = 1300;
const int MAX_OBSTACLE_SIZE = HEIGHT/5;
const Real  RATIO = (float)WIDTH / (float)HEIGHT;
const Real  BODY_SIZE = 0.02;
const vec3 PRED_COLOR = { 1., 0., 0. };
const vec3 BIRD_COLOR = { 1., 1., 1. };
const vec3 OBSTACLE_COLOR = { 0.2, 0.2, 0.2 };

// Laws parameters
const Real SEPARATION_RANGE = 2*sqrt(5)*BODY_SIZE*WIDTH/9;
const Real DEAD_RANGE = SEPARATION_RANGE/4;
const Real ALIGNMENT_RANGE = 3*SEPARATION_RANGE;
const Real COHESION_RANGE = 2*ALIGNMENT_RANGE;
const Real PREDATOR_RANGE = COHESION_RANGE;

const Real SEPARATION_RELAXATION = 0.5;
const Real ALIGNMENT_RELAXATION = 0.9;
const Real COHESION_RELAXATION = 0.9;
const Real PREDATOR_RELAXATION = 0.9;
const Real OBSTACLE_RELAXATION = 0.9;

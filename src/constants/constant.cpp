#include "constant.h"

const size_t DEFAULT_NUM_AGENTS = 500;
const size_t DEFAULT_NUM_PREDATORS = 3;
const size_t DEFAULT_NUM_OBSTACLES = 10;
const Real CLOSE = 50;
const Real SPEED = 2;
const Real PI  = 3.14159265358979323846;

// Display parameters
const int WIDTH = 2400;
const int HEIGHT = 1400;
const int MAX_OBSTACLE_SIZE = 200;
const Real  RATIO = (float)WIDTH / (float)HEIGHT;
const Real  TRIANGLE_SIZE = 0.015;
const vec3 PRED_COLOR = { 1., 0., 0. };
const vec3 BIRD_COLOR = { 1., 1., 1. };
const vec3 OBSTACLE_COLOR = { 0., 0., 1. };

// Laws parameters
const Real SEPARATION = TRIANGLE_SIZE*WIDTH;
const Real ALIGNMENT = 2*SEPARATION;
const Real PREDATOR = 50;
const Real COHESION = 200;

const Real SMOOTH_SEPARATION = 0.75;
const Real SMOOTH_ALIGNMENT = 0.9;
const Real SMOOTH_COHESION = 0.9;
const Real SMOOTH_PREDATOR = 0.9;
const Real SMOOTH_OBSTACLE = 0.9;

#include "constant.h"

size_t const DEFAULT_NUM_AGENTS = 500;
size_t const DEFAULT_NUM_PREDATORS = 3;
size_t const DEFAULT_NUM_OBSTACLES = 10;
double const CLOSE = 50;
double const SPEED = 2;
double const PI  = 3.14159265358979323846;

// Display parameters
const int WIDTH = 2400;
const int HEIGHT = 1400;
const int MAX_OBSTACLE_SIZE = 200;
const float RATIO = (float)WIDTH / (float)HEIGHT;
const double TRIANGLE_SIZE = 0.015;

// Laws parameters
double const SEPARATION = TRIANGLE_SIZE*WIDTH;
double const ALIGNMENT = 2*SEPARATION;
double const PREDATOR = 50;
double const COHESION = 200;

double const SMOOTH_SEPARATION = 0.75;
double const SMOOTH_ALIGNMENT = 0.9;
double const SMOOTH_COHESION = 0.9;
double const SMOOTH_PREDATOR = 0.9;
double const SMOOTH_OBSTACLE = 0.9;

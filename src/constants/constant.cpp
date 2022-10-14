#include "constant.h"

size_t const DEFAULT_NUM_AGENTS = 200;
size_t const DEFAULT_NUM_PREDATORS = 2;
double const CLOSE = 50;
double const SPEED = 2;
double const PI  = 3.14159265358979323846;

// Display parameters
const int WIDTH = 1800;
const int HEIGHT = 1200;
const int MAX_OBSTACLE_SIZE = 200;
const float RATIO = (float)WIDTH / (float)HEIGHT;
const double TRIANGLE_SIZE = 0.015;

// Laws parameters
double const SEPARATION = TRIANGLE_SIZE*WIDTH;
double const ALIGNMENT = 2*SEPARATION;
double const PREDATOR = 50;
double const COHESION = 200;

double const SMOOTH_ALIGNMENT = 0.25;
double const SMOOTH_COHESION = 0.5;

bool const TURN_AROUND = false;

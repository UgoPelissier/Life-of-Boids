#pragma once
#include <stdio.h>

// Agent parameters
extern size_t const DEFAULT_NUM_AGENTS;
extern size_t const DEFAULT_NUM_PREDATORS;
extern size_t const DEFAULT_NUM_OBSTACLES;
extern double const CLOSE;
extern double const SPEED;
extern double const PI;

// Display parameters
extern const int WIDTH;
extern const int HEIGHT;
extern const int MAX_OBSTACLE_SIZE;
extern const float RATIO;
extern const double TRIANGLE_SIZE;

// Laws parameters
extern double const SEPARATION;
extern double const ALIGNMENT;
extern double const COHESION;
extern double const PREDATOR;

extern double const SMOOTH_SEPARATION;
extern double const SMOOTH_ALIGNMENT;
extern double const SMOOTH_COHESION;
extern double const SMOOTH_PREDATOR;
extern double const SMOOTH_OBSTACLE;

extern bool const TURN_AROUND;
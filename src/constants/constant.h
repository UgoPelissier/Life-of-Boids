#pragma once
#include <stdio.h>

#include <array>
#include <cmath>

using Real = float;
using vec2 = std::array<float, 2>;
using vec3 = std::array<float, 3>;
using vec4 = std::array<float, 4>;
using mat2x2 = std::array<vec2, 2>;
using mat4x4 = std::array<vec4, 4>;

// Agent parameters
extern const size_t DEFAULT_NUM_AGENTS;
extern const size_t  DEFAULT_NUM_PREDATORS;
extern const size_t  DEFAULT_NUM_OBSTACLES;
extern const Real CLOSE;
extern const Real SPEED;
extern const Real PI;
extern const vec3 PRED_COLOR;
extern const vec3 BIRD_COLOR;
extern const vec3 OBSTACLE_COLOR;

// Display parameters
extern const int WIDTH;
extern const int HEIGHT;
extern const int MAX_OBSTACLE_SIZE;
extern const Real RATIO;
extern const Real TRIANGLE_SIZE;

// Laws parameters
extern const Real SEPARATION;
extern const Real ALIGNMENT;
extern const Real COHESION;
extern const Real PREDATOR;

extern const Real SMOOTH_SEPARATION;
extern const Real SMOOTH_ALIGNMENT;
extern const Real SMOOTH_COHESION;
extern const Real SMOOTH_PREDATOR;
extern const Real SMOOTH_OBSTACLE;
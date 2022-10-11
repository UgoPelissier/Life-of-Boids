#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>

//====================DEFAULT PARAMETERS DEFINED==================
#define DEFAULT_NUM_AGENTS 100

#define ANGLE_MIN -135
#define ANGLE_MAX 135

#define SPEED_MIN 0
#define SPEED_MAX 1

#define DEFAULT_COHESION_WEIGHT 0.5
#define DEFAULT_SEPARATION_WEIGHT 0.5 // replace with (1 - DEFAULT_COHESION_WEIGHT)

#define WIDTH 1200
#define HEIGHT 1000 // Window size parameter

#define triangleSize    0.025
#define PI  3.14159265358979323846
//====================DEFAULT PARAMETERS DEFINED END==================

//====================USEFUL FUNCTIONS================================
using vec2 = std::array<float, 2>;

inline double distance(vec2 p1, vec2 p2) {
    double d(0);
    d = std::sqrt( (p2[0]-p1[0])*(p2[0]-p1[0]) +  (p2[1]-p1[1])*(p2[1]-p1[1]) );
    return d;
}

inline bool overlap(vec2 c, std::vector<vec2> centers) {
    float h = triangleSize*std::sqrt(3)/2;
    float l = 2*h/3;
    for (auto& center : centers) {
        if (distance(c, center)<2*l) {
            return true;
        }
    }
    return false;
}

inline bool outsideWindow(vec2 center, float ratio) {
    float h = triangleSize*std::sqrt(3)/2;
    float l = 2*h/3;
    if ( std::abs(center[0])>ratio-l or std::abs(center[1])>1-l ) {
        return true;
    }
    return false;
}

inline std::tuple<std::vector<vec2>, std::vector<double>, std::vector<double>> initialiazeAgents() {

    std::vector<vec2> points;
    vec2 randomPoint;

    std::vector<double> angles;
    double randomAngle;

    std::vector<double> velocities;
    double randomVelocity;

    float ratio = (float)WIDTH / (float)HEIGHT;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::mt19937_64 rng;

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomPoint = {(float)(ratio*((2*unif(rng))-1)), (float)((2*unif(rng))-1)};
        randomAngle = 2*PI*unif(rng);
        randomVelocity = 2*unif(rng)-1;

        if ( (not overlap(randomPoint, points)) and (not outsideWindow(randomPoint, ratio)) ) {
            points.push_back(randomPoint);
            angles.push_back(randomAngle);
            velocities.push_back(randomVelocity);
        }
    }

    return std::make_tuple(points, angles, velocities);
}

//====================USEFUL FUNCTIONS END============================

class Vec {

private:
	//coordinates of the agent
	double x;
	double y;
	double angle; // alignment angle of the agent
	double velocity; // speed of the agent

public:
	Vec(); // default constructor
	Vec(int x, int y, double angle, double velocity); // parameterized constructor
	int& X(); // get x by reference
	int& Y(); // get y by reference
	double& theta(); // get angle by reference
	double& speed(); // get speed by reference
	Vec operator+(Vec b); // vector + vector
	Vec operator-(Vec b); // vector - vector
	Vec operator*(Vec b); // vector * vector (dot product)
	Vec operator*(double b); // vector * scalar

};

class Agent {
private:
	Vec vec;
public:
	Agent(int x, int y, double angle, double velocity);
	bool update_vector(int x, int y, double angle, double velocity);
};


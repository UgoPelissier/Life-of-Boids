#pragma once

//====================DEFAULT PARAMETERS DEFINED==================
#define DEFAULT_NUM_AGENTS 1

#define ANGLE_MIN -135
#define ANGLE_MAX 135

#define SPEED_MIN 0
#define SPEED_MAX 1

#define DEFAULT_COHESION_WEIGHT 0.5
#define DEFAULT_SEPARATION_WEIGHT 0.5 // replace with (1 - DEFAULT_COHESION_WEIGHT)

//====================DEFAULT PARAMETERS DEFINED END==================
class Vec {

private:
	//coordinates of the agent
	int x;
	int y;
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


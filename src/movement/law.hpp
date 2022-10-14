#include "../common.hpp"
#include <iostream>
#include <vector>

#define RANGE_SEPARATION 30
#define RANGE_ALIGMENT_LOW 30
#define RANGE_ALIGMENT_HIGH 70
#define RANGE_COHESION_LOW 60
#define RANGE_COHESION_HIGH 80
#define SPEED 1
#define WEIGHT_SEPARATION 0.12
#define WEIGHT_COHESION 0.05

double distance_agent(Agent agent1, Agent agent2);
void next_pos(int index, double angle, int corr_x, int corr_y, std::vector<Agent>& vect_agent);
bool verify_angle(Agent& agent_current, Agent& agent_neighbour);
void pilot(int index, std::vector<int> vec_separation, std::vector<int> vec_cohesion, std::vector<int> vec_aligment, std::vector<Agent>& vect_agent);
void law_function(std::vector<Agent>& vect_agent);

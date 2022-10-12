#include "../common.hpp"
#include <iostream>
#include <vector>

#define RANGE_SEPARATION 25
#define RANGE_ALIGMENT_LOW 25
#define RANGE_ALIGMENT_HIGH 50
#define RANGE_COHESION_LOW 50
#define RANGE_COHESION_HIGH 100

double distance_agent(Agent agent1, Agent agent2);
void next_pos(int index, double angle, int corr_x, int corr_y, std::vector<Agent>& vect_agent);
void pilot(int index, std::vector<int> vec_separation, std::vector<int> vec_cohesion, std::vector<int> vec_aligment, std::vector<Agent>& vect_agent);
void law_function(std::vector<Agent>& vect_agent);

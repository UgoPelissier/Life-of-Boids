#include "../common.hpp"
#include <iostream>
#include <vector>

#define RANGE_SEPARATION 50
#define RANGE_ALIGMENT_LOW 50
#define RANGE_ALIGMENT_HIGH 100
#define RANGE_COHESION_LOW 100
#define RANGE_COHESION_HIGH 150

double distance_agent(Agent agent1, Agent agent2);
void next_pos(Agent& agent, double angle, int corr_x, int corr_y);
void pilot(Agent& agent, std::vector<Agent> vec_separation, std::vector<Agent> vec_cohesion, std::vector<Agent> vec_aligment);
void law_function(std::vector<Agent>& vect_agent);

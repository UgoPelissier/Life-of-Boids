#include "../common.hpp"
#include <iostream>
#include <vector>



double distance_agent(Agent agent1, Agent agent2);
void next_pos(int index, double angle, int corr_x, int corr_y, std::vector<Agent>& vect_agent);
void pilot(int index, std::vector<int> vec_separation, std::vector<int> vec_cohesion, std::vector<int> vec_aligment, std::vector<Agent>& vect_agent);
void law_function(std::vector<Agent>& vect_agent);

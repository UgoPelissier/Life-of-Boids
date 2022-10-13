
#include "../common.hpp"
#include "law.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>



double distance_agent(Agent agent1, Agent agent2) {
	return std::sqrt((agent1.get_x() - agent2.get_x()) * (agent1.get_x() - agent2.get_x()) +
		(agent1.get_y() - agent2.get_y()) * (agent1.get_y() - agent2.get_y()));
}

void next_pos(int index, double angle, int corr_x, int corr_y, std::vector<Agent>& vect_agent) {

	vect_agent[index].update(vect_agent[index].get_x() + corr_x + round(vect_agent[index].get_speed() * cos(angle)),
		vect_agent[index].get_y() + corr_y + round(vect_agent[index].get_speed() * sin(angle)),
		angle);
}

void pilot(int index, std::vector<int> vec_separation, std::vector<int> vec_cohesion, std::vector<int> vec_aligment, std::vector<Agent>& vect_agent) {
	double angle = .0;
	double correction_angle = .0;
	if (!vec_aligment.empty()) {
		for each (int i in vec_aligment) {
			angle = angle + vect_agent[i].get_angle();
		}
		angle = angle / vec_aligment.size();
		correction_angle = vect_agent[index].get_angle() + (angle - vect_agent[index].get_angle())*RELAXATION*WEIGHT_ALIGMENT;
		

	}

	int x = 0;
	int y = 0;
	int correction_x = 0;
	int correction_y = 0;
	if (!vec_cohesion.empty()) {
		for each (int j in vec_cohesion) {
			x = x + vect_agent[j].get_x();
			y = y + vect_agent[j].get_y();
		}
		x = (int)x / vec_cohesion.size();
		y = (int)y / vec_cohesion.size();

		correction_x = (int)(x - vect_agent[index].get_x()) * RELAXATION*WEIGHT_COHESION;
		correction_y = (int)(y - vect_agent[index].get_y()) * RELAXATION*WEIGHT_COHESION;
	}
	x = 0;
	y = 0;
	if (!vec_separation.empty()) {
		for each (int k in vec_separation) {
			x = x + vect_agent[k].get_x();
			y = y + vect_agent[k].get_y();
		}
		x = (int)x / vec_separation.size();
		y = (int)y / vec_separation.size();

		correction_x = correction_x + (int)(-(x - vect_agent[index].get_x()) * RELAXATION*WEIGHT_SEPARATION);
		correction_y = correction_y + (int)(-(y - vect_agent[index].get_y()) * RELAXATION*WEIGHT_SEPARATION);
	}

	next_pos(index, correction_angle, correction_x, correction_y, vect_agent);
}

void law_function(std::vector<Agent>& vect_agent) {

	for (int i = 0; i < vect_agent.size(); i++) {
		std::vector<int> vec_separation;
		std::vector<int> vec_cohesion;
		std::vector<int> vec_aligment;
		vec_separation = {};
		vec_cohesion = {};
		vec_aligment = {};
		bool test_flee;
		test_flee = false;
		if (vect_agent[i].get_type() == 1) { //agents 1 want to flee agents 2
			vect_agent[i].get_speed() = SPEED;
			for (int j = 0; j < vect_agent.size(); j++) {
				double distance = distance_agent(vect_agent[i], vect_agent[j]);
				if (vect_agent[j].get_type() == 2) {
					if (distance < RANGE_SEPARATION+50) {
						vec_separation.push_back(j);
						test_flee = true;
						vect_agent[i].get_speed() = SPEED * 3;
					}
					/*
					if (distance < RANGE_ALIGMENT_HIGH && distance > RANGE_ALIGMENT_LOW) {
						vec_aligment.push_back(j);
						test_flee = true;
						vect_agent[i].get_speed() = SPEED * 3;
					}*/
				}
				else if (test_flee==false) {
					if (distance < RANGE_SEPARATION) {
						vec_separation.push_back(j);
					}
					if (distance < RANGE_COHESION_HIGH && distance > RANGE_COHESION_LOW) {
						vec_cohesion.push_back(j);
					}
					if (distance < RANGE_ALIGMENT_HIGH && distance > RANGE_ALIGMENT_LOW) {
						vec_aligment.push_back(j);
					}
				}
				
			}
		}
		else { //agents 2 want to get agents 1
			for (int j = 0; j < vect_agent.size(); j++) {
				double distance = distance_agent(vect_agent[i], vect_agent[j]);

				if (vect_agent[j].get_type() == 1) {
					if (distance < vect_agent[i].get_cohesion_high()) {
						vec_cohesion.push_back(j);
						vec_aligment.push_back(j);
					}
					/*if (distance < RANGE_ALIGMENT_HIGH && distance > RANGE_ALIGMENT_LOW) {
						vec_aligment.push_back(j);
					}*/

				}/*
				else {
					if (distance < vect_agent[i].get_separation()) {
						vec_separation.push_back(j);
					}
					if (distance < vect_agent[i].get_cohesion_high() && distance > vect_agent[i].get_cohesion_low()) {
						vec_cohesion.push_back(j);
					}
					if (distance < RANGE_ALIGMENT_HIGH && distance > RANGE_ALIGMENT_LOW) {
						vec_aligment.push_back(j);
					}

				}*/
				
				
			}

		}

		
		pilot(i, vec_separation, vec_cohesion, vec_aligment, vect_agent);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

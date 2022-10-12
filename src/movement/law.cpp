
#include "../common.hpp"
#include "law.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>

#define RANGE_SEPARATION 50
#define RANGE_ALIGMENT_LOW 50
#define RANGE_ALIGMENT_HIGH 100
#define RANGE_COHESION_LOW 80
#define RANGE_COHESION_HIGH 150
#define SPEED 1

double distance_agent(Agent agent1, Agent agent2) {
	return std::sqrt((agent1.get_x() - agent2.get_x()) * (agent1.get_x() - agent2.get_x()) +
		(agent1.get_y() - agent2.get_y()) * (agent1.get_y() - agent2.get_y()));
}

void next_pos(int index, double angle, int corr_x, int corr_y, std::vector<Agent>& vect_agent) {
	//std::cout << agent.get_x() << std::endl;
	vect_agent[index].update(vect_agent[index].get_x() + corr_x + round(SPEED * cos(angle)),
		vect_agent[index].get_y() + corr_y + round(SPEED * sin(angle)),
		angle);
	std::cout << "next pos " << vect_agent[index].get_x() << std::endl;
}

void pilot(int index, std::vector<int> vec_separation, std::vector<int> vec_cohesion, std::vector<int> vec_aligment, std::vector<Agent>& vect_agent) {
	double angle = .0;
	if (!vec_aligment.empty()) {
		for each (int i in vec_aligment) {
			angle = angle + vect_agent[i].get_angle();
		}
		angle = angle / vec_aligment.size();
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

		correction_x = (int)(x - vect_agent[index].get_x()) * 0.1;
		correction_y = (int)(y - vect_agent[index].get_y()) * 0.1;
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

		correction_x = correction_x + (int)(-(x - vect_agent[index].get_x()) * 0.15);
		correction_y = correction_y + (int)(-(y - vect_agent[index].get_y()) * 0.15);
	}
	//std::cout << "pilot " << angle << " " << correction_x << " " << correction_y << std::endl;

	next_pos(index, angle, correction_x, correction_y, vect_agent);
}

void law_function(std::vector<Agent>& vect_agent) {
	//assert(vect_agent.size() > 0);

	for (int i = 0; i < vect_agent.size(); i++) {
		std::vector<int> vec_separation;
		std::vector<int> vec_cohesion;
		std::vector<int> vec_aligment;
		vec_separation = {};
		vec_cohesion = {};
		vec_aligment = {};
		for (int j = 0; j < vect_agent.size(); j++) {
			double distance = distance_agent(vect_agent[i], vect_agent[j]);
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
		pilot(i, vec_separation, vec_cohesion, vec_aligment, vect_agent);
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


#include "../common.hpp"
#include "law.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <cmath>

inline double degree2radian(double degree) {
	return degree * PI / 180;
}

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
	double angle = 0.0;
	if (!vec_aligment.empty()) {
		for (int i : vec_aligment) {
			angle = angle + vect_agent[i].get_angle();
		}
		angle = angle / vec_aligment.size();
	}

	int x = 0;
	int y = 0;
	int correction_x = 0;
	int correction_y = 0;
	if (!vec_cohesion.empty()) {
		for (int j : vec_cohesion) {
			x = x + vect_agent[j].get_x();
			y = y + vect_agent[j].get_y();
		}
		x = (int)x / vec_cohesion.size();
		y = (int)y / vec_cohesion.size();

		correction_x = (int)(x - vect_agent[index].get_x()) * WEIGHT_COHESION;
		correction_y = (int)(y - vect_agent[index].get_y()) * WEIGHT_COHESION;
	}
	x = 0;
	y = 0;
	if (!vec_separation.empty()) {
		for (int k : vec_separation) {
			x = x + vect_agent[k].get_x();
			y = y + vect_agent[k].get_y();
		}
		x = (int)x / vec_separation.size();
		y = (int)y / vec_separation.size();

		correction_x = correction_x + (int)(-(x - vect_agent[index].get_x()) * WEIGHT_SEPARATION);
		correction_y = correction_y + (int)(-(y - vect_agent[index].get_y()) * WEIGHT_SEPARATION);
	}
	//std::cout << "pilot " << angle << " " << correction_x << " " << correction_y << std::endl;
	if (angle == 0.0) {
		angle = vect_agent[index].get_angle();
	}
	next_pos(index, angle, correction_x, correction_y, vect_agent);
}

bool verify_angle(Agent& agent_current, Agent& agent_neighbour) {
	int distance_y = agent_current.get_y() - agent_neighbour.get_y();
	int distance_x = agent_current.get_x() - agent_neighbour.get_x();
	/*if ((distance_x == 0 || distance_y == 0) && agent_current.get_angle() == agent_neighbour.get_angle()) {
		return false;
	}*/
	bool satisfied_x = distance_x == 0 && distance_y < 0;
	bool satisfied_y = distance_y == 0 && distance_x < 0;
	bool same_angle = agent_current.get_angle() == agent_neighbour.get_angle();
	if ((satisfied_x || satisfied_y) && same_angle) {
		return true;
	}
	if ((distance_x == 0 || distance_y == 0)) {
		Agent next_current_agent(agent_current.get_x() + round(SPEED * cos(agent_current.get_angle())), 
			agent_current.get_y() + round(SPEED * cos(agent_current.get_angle())), 0, 0);
		Agent next_neighbour_agent(agent_neighbour.get_x() + round(SPEED * cos(agent_neighbour.get_angle())),
			agent_neighbour.get_y() + round(SPEED * cos(agent_neighbour.get_angle())), 0, 0);
		if (distance_agent(agent_current, agent_neighbour) > distance_agent(next_current_agent, next_neighbour_agent)) {
			return true;
		}
		else {
			return false;
		}
	}
	double angle = abs(asin(distance_y / distance_x)) + agent_current.get_angle();
	//std::cout << "angle " << angle << std::endl;
	return angle < degree2radian(ANGLE_MAX);
}

void law_function(std::vector<Agent>& vect_agent) {
	//assert(vect_agent.size() > 0);

	for (size_t i = 0; i < vect_agent.size(); i++) {
		std::vector<int> vec_separation;
		std::vector<int> vec_cohesion;
		std::vector<int> vec_aligment;
		vec_separation = {};
		vec_cohesion = {};
		vec_aligment = {};
		for (int j = 0; j < vect_agent.size(); j++) {
			double distance = distance_agent(vect_agent[i], vect_agent[j]);
			//if (true) {
			if (verify_angle(vect_agent[i], vect_agent[j])) {
				
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
		//std::this_thread::sleep_for(std::chrono::milliseconds(20));
		pilot(i, vec_separation, vec_cohesion, vec_aligment, vect_agent);
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

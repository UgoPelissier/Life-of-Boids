
#include "../common.hpp"
#include <iostream>
#include <vector>

#define RANGE_SEPARATION 50
#define RANGE_ALIGMENT_LOW 50
#define RANGE_ALIGMENT_HIGH 100
#define RANGE_COHESION_LOW 80
#define RANGE_COHESION_HIGH 150

void law_function(std::vector<Agent> vect_agent) {
	for each (Agent agent in vect_agent) {
		std::vector<Agent> vec_separation;
		std::vector<Agent> vec_cohesion;
		std::vector<Agent> vec_aligment;
		vec_separation = {};
		vec_cohesion = {};
		vec_aligment = {};

		for each (Agent agent1 in vect_agent) {
			double distance = distance_agent(agent, agent1);
			if ( distance < RANGE_SEPARATION) {
				vec_separation.push_back(agent1);
			}
			if (distance < RANGE_COHESION_HIGH && distance > RANGE_COHESION_LOW) {
				vec_cohesion.push_back(agent1);
			}
			if (distance < RANGE_ALIGMENT_HIGH && distance > RANGE_ALIGMENT_LOW) {
				vec_aligment.push_back(agent1);
			}
		}

		pilot( agent, vec_separation, vec_cohesion, vec_aligment);
	}
}

void pilot(Agent agent, std::vector<Agent> vec_separation, std::vector<Agent> vec_cohesion, std::vector<Agent> vec_aligment) {
	double angle = .0;
	for each (Agent agent1 in vec_aligment) {
		angle = angle + agent1.theta();
	}
	angle = angle / vec_aligment.size();

	int x = 0;
	int y = 0;
	int correction_x = 0;
	int correction_y = 0;

	for each (Agent agent2 in vec_cohesion){
		x = x + agent2.X();
		y = y + agent2.Y();
	}
	x = (int) x / vec_cohesion.size();
	y = (int) y / vec_cohesion.size();

	correction_x = (int) (x - agent.X()) * 0.1;
	correction_y = (int) (y - agent.Y()) * 0.1;

	int x = 0;
	int y = 0;

	for each (Agent agent3 in vec_separation) {
		x = x + agent3.X();
		y = y + agent3.Y();
	}
	x = (int) x / vec_cohesion.size();
	y = (int) y / vec_cohesion.size();

	correction_x = correction_x + (int)(-(x - agent.X()) * 0.3);
	correction_y = correction_y + (int)(-(y - agent.Y()) * 0.3);

	next_pos(agent, angle, correction_x, correction_y);
}

double distance_agent(Agent agent1, Agent agent2) {
	return std::sqrt((agent1.X() - agent2.X()) * (agent1.X() - agent2.X()) +
					(agent1.Y() - agent2.Y()) * (agent1.Y() - agent2.Y()));
}

void next_pos(Agent agent, double angle, int corr_x, int corr_y) {
	agent.update_vector(agent.X() + corr_x + (int)agent.speed() * cos(angle),
						agent.Y() + corr_y + (int)agent.speed() * sin(angle),
						angle,
						agent.speed());
}





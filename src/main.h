# pragma once

#include "config/config.h"
#include "display/display.h"
#include "movement/agents/agent.h"
#include "movement/agents/predator.h"
#include "movement/agents/bird.h"
#include "movement/obstacles/obstacle.h"
#include "movement/fruits/fruit.h"
#include "movement/fruits/tree.h"

namespace vars {

    struct agentVars_t {
        triangle::vertices_t trianglesObs,
            trianglesPredators,
            trianglesBirds,
            trianglesTree,
            trianglesFruit;
        std::vector<Obstacle> obstacles;
        predators_t predators;
        birds_t birds;
        std::vector<Tree> trees;
        std::vector<Fruit> fruits;
    };
    using agentWindowVars_t = struct agentVars_t;
};

std::vector<Fruit> updateObjects(std::vector<Obstacle>& obstacles,
                                predators_t& predators,
                                birds_t& birds,
                                std::vector<Tree>& trees,
                                std::vector<Fruit>& fruits);

vars::agentWindowVars_t initAgentWindow();
void updateAgentWindow(GLFWwindow* window, vars::agentWindowVars_t &var);
void addAgent(GLFWwindow* window, vars::agentWindowVars_t& var);
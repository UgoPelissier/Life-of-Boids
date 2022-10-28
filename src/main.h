# pragma once

#include "config/config.h"
#include "display/display.h"
#include "movement/agents/agent.h"
#include "movement/agents/predator.h"
#include "movement/agents/bird.h"
#include "movement/obstacles/obstacle.h"
#include "movement/fruits/fruit.h"
#include "movement/fruits/tree.h"




std::vector<Fruit> updateObjects(std::vector<Obstacle>& obstacles,
    predators_t& predators,
    birds_t& birds,
    std::vector<Tree>& trees,
    std::vector<Fruit>& fruits);

std::tuple<
    std::vector<Obstacle>,
    predators_t,
    birds_t,
    std::vector<Tree>,
    std::vector<Fruit>,
    triangle::vertices_t,
    triangle::vertices_t,
    triangle::vertices_t,
    triangle::vertices_t,
    triangle::vertices_t
> initAgentWindow();

void updateAgentWindow(
    GLFWwindow* window,
    std::vector<Obstacle>& obstacles,
    predators_t& predators,
    birds_t& birds,
    std::vector<Tree>& trees,
    std::vector<Fruit>& fruits,
    triangle::vertices_t& trianglesPredators,
    triangle::vertices_t& trianglesBirds,
    triangle::vertices_t& trianglesFruit
);

void addAgent(
    GLFWwindow* window,
    std::vector<Obstacle>& obstacles,
    predators_t& predators,
    birds_t& birds,
    triangle::vertices_t& trianglesPredators,
    triangle::vertices_t& trianglesBirds
);
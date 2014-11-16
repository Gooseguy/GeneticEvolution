//
//  SoftBodyAgent.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include "PhysicsObject.h"
#include <vector>
#include "glm/glm.hpp"
#include "SoftBodyNode.h"
class SoftBodyAgent
{
public:
    SoftBodyAgent(glm::vec3 pos);
    static const int INITIAL_CUBE_WIDTH = 5;
    static const float NODE_SPACING;
    void Update(float timeStep);
    std::vector<SoftBodyNode*> nodes;
    std::vector<Spring> springs;
private:
    inline int initialNodeIndex(int x, int y, int z);
    void addSpring(SoftBodyNode* node1, SoftBodyNode* node2);
    void addSpring(int node1, int node2);
};

int SoftBodyAgent::initialNodeIndex(int x, int y, int z)
{
    return z + y * INITIAL_CUBE_WIDTH + x * INITIAL_CUBE_WIDTH * INITIAL_CUBE_WIDTH;
}
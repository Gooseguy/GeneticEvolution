//
//  SoftBodyAgent.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "SoftBodyAgent.h"
#include <OpenGL/gl3.h>
#include "glm/gtx/euler_angles.hpp"
const float SoftBodyAgent::NODE_SPACING = 0.1f;

SoftBodyAgent::SoftBodyAgent(glm::vec3 pos)
{
    for (int i = 0; i<INITIAL_CUBE_WIDTH;++i)
        for (int j = 0; j<INITIAL_CUBE_WIDTH;++j)
            for (int k = 0; k<INITIAL_CUBE_WIDTH;++k)
            {
                nodes.push_back(new SoftBodyNode(pos + glm::mat3(glm::eulerAngleXZ(30*3.141f/180,0.f)) * glm::vec3(i*NODE_SPACING,j*NODE_SPACING,k*NODE_SPACING)));
            }
    for (int i = 1; i<INITIAL_CUBE_WIDTH-1;++i)
        for (int j = 0; j<INITIAL_CUBE_WIDTH;++j)
            for (int k = 0; k<INITIAL_CUBE_WIDTH;++k)
            {
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i+1, j, k));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i-1, j, k));
            }
    
    for (int i = 0; i<INITIAL_CUBE_WIDTH;++i)
        for (int j = 1; j<INITIAL_CUBE_WIDTH-1;++j)
            for (int k = 0; k<INITIAL_CUBE_WIDTH;++k)
            {
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j+1, k));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j-1, k));
            }
    for (int i = 0; i<INITIAL_CUBE_WIDTH;++i)
        for (int j = 0; j<INITIAL_CUBE_WIDTH;++j)
            for (int k = 1; k<INITIAL_CUBE_WIDTH-1;++k)
            {
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j, k+1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j, k-1));
            }
    
    for (int i = 1; i<INITIAL_CUBE_WIDTH-1;++i)
        for (int j = 1; j<INITIAL_CUBE_WIDTH-1;++j)
            for (int k = 0; k<INITIAL_CUBE_WIDTH;++k)
            {
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i+1, j+1, k));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i+1, j-1, k));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i-1, j-1, k));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i-1, j+1, k));
            }
    for (int i = 0; i<INITIAL_CUBE_WIDTH;++i)
        for (int j = 1; j<INITIAL_CUBE_WIDTH-1;++j)
            for (int k = 1; k<INITIAL_CUBE_WIDTH-1;++k)
            {
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j+1, k+1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j+1, k-1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j-1, k+1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i, j-1, k-1));
            }
    for (int i = 1; i<INITIAL_CUBE_WIDTH-1;++i)
        for (int j = 0; j<INITIAL_CUBE_WIDTH;++j)
            for (int k = 1; k<INITIAL_CUBE_WIDTH-1;++k)
            {
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i+1, j, k+1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i+1, j, k-1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i-1, j, k+1));
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i-1, j, k-1));
            }
    
}

void SoftBodyAgent::Update(float timeStep)
{
    for (auto& spring : springs)
    {
        spring.ApplyForces();
    }
    for (auto& node : nodes)
    {
        node->Update(timeStep);
    }
}


void SoftBodyAgent::addSpring(SoftBodyNode *node1, SoftBodyNode *node2)
{
    std::size_t idx = springs.size();
    springs.push_back(Spring(node1, node2));
    node1->AddSpring(&springs[idx]);
    node2->AddSpring(&springs[idx]);
}

void SoftBodyAgent::addSpring(int node1, int node2)
{
    addSpring(nodes[node1], nodes[node2]);
}
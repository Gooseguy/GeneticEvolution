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
#include "Spring.h"
#include "glm/gtx/norm.hpp"
class SoftBodyAgent
{
public:
    SoftBodyAgent(glm::vec3 pos, glm::vec3 color);
    SoftBodyAgent(const SoftBodyAgent& agent);
    ~SoftBodyAgent();
    static int INITIAL_CUBE_WIDTH;
    static float NODE_SPACING;
    void Update(float timeStep, int currentTime);
    std::vector<SoftBodyNode> nodes;
    std::vector<Spring> springs;
    std::vector<glm::vec3> initialPositions;
    void Mutate();
    void RemoveNode(std::size_t node);
    float TotalMinimumHeight;
    float TotalDistance;
    glm::vec3 color;
    float Size;
    glm::vec3 StartingPos;
    inline glm::vec3 GetNodeColor(SoftBodyNode& node);
    float TotalEnergy;
private:
    void mutateGeometry();
    inline int initialNodeIndex(int x, int y, int z);
    void addSpring(std::size_t node1, std::size_t node2);
    inline void addSpringDisplacement(int x, int y, int z);
    void getSize();
};

glm::vec3 SoftBodyAgent::GetNodeColor(SoftBodyNode& node)
{
    float mult = node.TotalStress;
    node.TotalStress=0;
    return color+mult*0.00001f;
}

int SoftBodyAgent::initialNodeIndex(int x, int y, int z)
{
    return z + y * INITIAL_CUBE_WIDTH + x * INITIAL_CUBE_WIDTH * INITIAL_CUBE_WIDTH;
}

void SoftBodyAgent::addSpringDisplacement(int x, int y, int z)
{
    for (int i = int(x<0); i<INITIAL_CUBE_WIDTH - int(x>0);++i)
        for (int j = int(y<0); j<INITIAL_CUBE_WIDTH - int(y>0);++j)
            for (int k = int(z<0); k<INITIAL_CUBE_WIDTH - int(z>0);++k)
                addSpring(initialNodeIndex(i, j, k), initialNodeIndex(i + x, j + y, k + z));
}
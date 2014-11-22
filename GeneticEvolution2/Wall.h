//
//  Wall.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/21/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "PhysicsObject.h"
#include <vector>
#include "Vertex.h"
class Wall
{
public:
    Wall(glm::vec3 origin, glm::vec3 normal, float frictionConstant);
    glm::vec3 Origin;
    glm::vec3 Normal;
    float FrictionConstant;
    void ApplyForce(PhysicsObject* object);
    static const float FORCE_MULT;
    void AppendVertices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
private:
    std::vector<Vertex> baseGrid;
    void constructBaseGrid();
    const float gridSquareLength;
    
};
//
//  Vertex.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/21/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    Vertex(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}
    Vertex(float x, float y, float z, float r, float g, float b) : position(x,y,z), color(r,g,b) {}
};
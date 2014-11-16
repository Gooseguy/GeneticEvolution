//
//  EvolutionSystem.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include <OpenGL/gl3.h>
#include "glm/glm.hpp"
#include <vector>
#include "SoftBodyAgent.h"
class EvolutionSystem
{
public:
    enum class RenderMode
    {
        POINT,
        WIRE,
    } CurrentRenderMode;
    struct Vertex
    {
        glm::vec3 position;
        Vertex(glm::vec3 pos) : position(pos) {}
        Vertex(float x, float y, float z) : position(x,y,z) {}
    };
    EvolutionSystem();
    void Draw();
    void Update();
    const float TIME_STEP;
    SoftBodyAgent* selectedAgent;
private:
    GLuint vao,vbo;
    void generateBuffers();
    void updateBuffers();
    std::vector<Vertex> vertices;
    std::vector<SoftBodyAgent> agents;
};
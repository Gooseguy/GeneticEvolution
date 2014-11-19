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
#include <string>
#include<functional>
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
        glm::vec3 color;
        Vertex(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}
        Vertex(float x, float y, float z, float r, float g, float b) : position(x,y,z), color(r,g,b) {}
    };
    EvolutionSystem(std::string _outputFileLocation);
    void Draw();
    void Update();
    const float TIME_STEP;
    const int NUM_AGENTS;
    SoftBodyAgent* selectedAgent;
private:
    GLuint vao,vbo;
    void generateBuffers();
    void updateBuffers();
    std::vector<Vertex> vertices;
    std::vector<SoftBodyAgent*> agents;
    std::vector<std::function<float(SoftBodyAgent&)>> performanceFunctions;
    std::size_t currentFunction;
    int currentTime;
    int generationLength;
    void nextGeneration();
    void configurePerformanceFunctions();
    std::string outputFileLocation;
};
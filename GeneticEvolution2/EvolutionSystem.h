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
#include "Wall.h"
#include "ConfigurationManager.h"

class EvolutionSystem
{
public:
    enum class RenderMode
    {
        POINT_ALL,
        WIRE_ALL,
        POINT_ONE,
        WIRE_ONE,
        SOLID_ONE,
        SOLID_ALL,
    } CurrentRenderMode;
    inline void NextRenderMode();
    EvolutionSystem(std::string _outputFileLocation, ConfigurationManager& configManager);
    void Draw();
    void Update();
    const float TIME_STEP;
    const int NUM_AGENTS;
    const float GRAVITATIONAL_ACCELERATION;
    const float DRAG_COEFFICIENT;
    const float NEW_AGENT_PROBABILITY;
    int selectedAgent;
    inline void NextSelectedAgent();
    inline void PrevSelectedAgent();
    inline void ToggleAccelerated();
    inline void IncreasePlaybackRate();
    inline void DecreasePlaybackRate();
    void SelectClosestAgent(glm::vec3 ref);
private:
    GLuint vao,vbo,ibo;
    void generateBuffers();
    void updateBuffers();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<SoftBodyAgent*> agents;
    std::vector<std::function<float(SoftBodyAgent&)>> performanceFunctions;
    std::size_t currentFunction;
    int currentTime;
    int generationLength;
    void nextGeneration();
    void configurePerformanceFunctions();
    std::string outputFileLocation;
    bool accelerate;
    bool accelerateNextRound;
    void updateAgent(SoftBodyAgent* agent);
    std::vector<Wall> walls;
    
    unsigned int playbackRate;
};

void EvolutionSystem::IncreasePlaybackRate()
{
    playbackRate++;
}

void EvolutionSystem::DecreasePlaybackRate()
{
    playbackRate--;
    if (playbackRate<1) playbackRate=1;
}

void EvolutionSystem::ToggleAccelerated()
{
    if (!accelerate) accelerateNextRound=true;
    else accelerate=false;
}

void EvolutionSystem::NextRenderMode()
{
    switch (CurrentRenderMode)
    {
        case RenderMode::POINT_ALL:
            CurrentRenderMode=RenderMode::WIRE_ALL;
            break;
        case RenderMode::WIRE_ALL:
            CurrentRenderMode=RenderMode::POINT_ONE;
            break;
        case RenderMode::POINT_ONE:
            CurrentRenderMode=RenderMode::WIRE_ONE;
            break;
        case RenderMode::WIRE_ONE:
            CurrentRenderMode=RenderMode::SOLID_ONE;
            break;
        case RenderMode::SOLID_ONE:
            CurrentRenderMode=RenderMode::POINT_ALL;
            break;
    //Not including "SOLID_ALL" because it is too slow.
//        case RenderMode::SOLID_ALL:
//            CurrentRenderMode=RenderMode::POINT_ALL;
//            break;
    }
}

void EvolutionSystem::NextSelectedAgent()
{
    selectedAgent++;
    if (selectedAgent>=NUM_AGENTS) selectedAgent=0;
}

void EvolutionSystem::PrevSelectedAgent()
{
    selectedAgent--;
    if (selectedAgent<0) selectedAgent=NUM_AGENTS-1;
}
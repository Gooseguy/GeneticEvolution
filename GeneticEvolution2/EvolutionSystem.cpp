//
//  EvolutionSystem.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "EvolutionSystem.h"
#include "RandomUtils.h"
#include <fstream>
EvolutionSystem::EvolutionSystem(std::string _outputFileLocation) : NUM_AGENTS(10),
TIME_STEP(0.0005),
CurrentRenderMode(RenderMode::POINT),
generationLength(8./TIME_STEP),
outputFileLocation(_outputFileLocation),
currentFunction(0)
{
    for (int i = 0; i<NUM_AGENTS;++i)
        agents.push_back(new SoftBodyAgent(glm::vec3(0,0,0.01), glm::vec3(RandomUtils::Instance.UniformFloat(),RandomUtils::Instance.UniformFloat(),RandomUtils::Instance.UniformFloat())));
    selectedAgent=agents[0];
    generateBuffers();
    updateBuffers();
}
static const EvolutionSystem::Vertex g_quad_vertex_buffer_data[] = {
    EvolutionSystem::Vertex(-1.0f, -1.0f, 0.0f, 1.f,0.f,0.f),
    EvolutionSystem::Vertex(1.0f, -1.0f, 0.0f, 1.f,0.f,0.f),
    EvolutionSystem::Vertex(-1.0f,  1.0f, 0.0f, 1.f,0.f,0.f),
    EvolutionSystem::Vertex(-1.0f,  1.0f, 0.0f, 1.f,0.f,0.f),
    EvolutionSystem::Vertex(1.0f, -1.0f, 0.0f, 1.f,0.f,0.f),
    EvolutionSystem::Vertex(1.0f,  1.0f, 0.0f, 1.f,0.f,0.f),
};
#include <iostream>
void EvolutionSystem::generateBuffers()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)__offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)__offsetof(Vertex, color));
    glBindVertexArray(0);
}

void EvolutionSystem::updateBuffers()
{
    vertices.clear();
    for (auto& vertex : g_quad_vertex_buffer_data) vertices.push_back(vertex);
    
    //regenerate vertices based on selected agent
//    if (selectedAgent!=nullptr)
//    {
    //        auto agent = selectedAgent;
    for (auto& agent : agents)
    {
        switch (CurrentRenderMode)
        {
            case RenderMode::POINT:
                for (auto& node : selectedAgent->nodes)
                {
                    vertices.push_back(Vertex(node.Position,agent->color));
                }
                break;
            case RenderMode::WIRE:
                for (auto& spring : selectedAgent->springs)
                {
                    vertices.push_back(Vertex(agent->nodes[spring.obj1].Position,agent->color));
                    vertices.push_back(Vertex(agent->nodes[spring.obj2].Position,agent->color));
                }
                break;
        }
    }
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void EvolutionSystem::Draw()
{
    updateBuffers();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(vao);
    if (vertices.size() > 0)
    {
        GLenum polyType;
        switch (CurrentRenderMode)
        {
            case RenderMode::POINT:
                polyType=GL_POINTS;
                break;
            case RenderMode::WIRE:
                polyType=GL_LINES;
                break;
        }
        glDrawArrays(polyType, 0, vertices.size());
    }
    
    glBindVertexArray(0);
    
}

void EvolutionSystem::Update()
{
    //apply external forces
    for (auto& agent : agents)
    {
        for (auto& node : agent->nodes)
        {
//            node.ApplyForce(glm::vec3(0,0,-1));
//            if (node->Position.x > 0.25) node->ApplyForce(glm::vec3(0,1,0));
            node.ApplyForce(glm::vec3(0,0,-1));
            if (node.Position.z < 0)
            {
                float normalForce =-node.Position.z*10000 - node.NetForce.z;
                const float k_friction = 0.9;
                node.ApplyForce(glm::vec3(0,0,normalForce));
                if (node.Velocity.x!=0 && node.Velocity.y!=0)
                node.ApplyForce(-normalForce * k_friction * glm::normalize(glm::vec3(node.Velocity.x,node.Velocity.y,0)));
            }
        }
    }
//    selectedAgent->nodes[0]->ApplyForce(glm::vec3(20,0,0));
    for (auto& agent : agents)
        agent->Update(TIME_STEP, currentTime*TIME_STEP*100);
    currentTime++;
    if (currentTime%generationLength==0) nextGeneration();
}

void EvolutionSystem::nextGeneration()
{
    std::ofstream stream(outputFileLocation, std::ios::out | std::ios::app);
    std::vector<float> intervals(agents.size(), 1.0);
    std::vector<float> probabilities(agents.size());
    float average=0;
//    float min=0;
    for (int i = 0; i<agents.size();++i)
    {
        probabilities[i] = performanceFunctions[currentFunction](*agents[i]);
//        if (probabilities[i]<min || min==0) min=probabilities[i];
    }
    
    for(auto& p : probabilities) {
//        p-=min;
        average+=p;
    }
//    for (auto& p :probabilities) p/=average;
    average/=probabilities.size();
    stream << currentTime/generationLength << "," << average << std::endl;
    for (float i : probabilities) std::cout << i << std::endl;
    std::piecewise_constant_distribution<float> distribution(intervals.begin(), intervals.end(), probabilities.begin());
    std::vector<SoftBodyAgent*> newAgents(agents.size());
    for (int i = 0; i<agents.size();++i)
    {
        newAgents[i] = new SoftBodyAgent(*agents[(int)distribution(RandomUtils::Instance.rand)]);
    }

    for (auto& agent : agents) delete agent;
    agents = newAgents;
    selectedAgent=agents[0];
}

void EvolutionSystem::configurePerformanceFunctions()
{
    performanceFunctions.push_back(std::function<float(SoftBodyAgent&)>([this](SoftBodyAgent& agent) {
        return 0.1f + agent.TotalMinimumHeight / generationLength;
    }));
}
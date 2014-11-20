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
#include <thread>

#include <iostream>
EvolutionSystem::EvolutionSystem(std::string _outputFileLocation) : NUM_AGENTS(50),
gridSquareLength(0.05),
TIME_STEP(0.0005),
CurrentRenderMode(RenderMode::POINT_ALL),
generationLength(8./TIME_STEP),
outputFileLocation(_outputFileLocation),
currentFunction(0),
accelerate(false),
selectedAgent(0)
{
    configurePerformanceFunctions();
    for (int i = 0; i<NUM_AGENTS;++i)
        agents.push_back(new SoftBodyAgent(glm::vec3(0,0,0.01), glm::vec3(RandomUtils::UniformFloat(),RandomUtils::UniformFloat(),RandomUtils::UniformFloat())));
    constructBaseGrid();
    generateBuffers();
    updateBuffers();
}


void EvolutionSystem::generateBuffers()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)__offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)__offsetof(Vertex, color));
    glBindVertexArray(0);
}

void EvolutionSystem::constructBaseGrid()
{
    for (float i = -1; i<=1+gridSquareLength;i+=gridSquareLength)
    {
        baseGrid.push_back(Vertex(-1,i,0,1.f,0,0));
        baseGrid.push_back(Vertex(1,i,0,1.f,0,0));
    }
    for (float i = -1; i<=1+gridSquareLength;i+=gridSquareLength)
    {
        baseGrid.push_back(Vertex(i,-1,0,1.f,0,0));
        baseGrid.push_back(Vertex(i,1,0,1.f,0,0));
    }
}

void EvolutionSystem::updateBuffers()
{
    vertices.clear();
    indices.clear();
    {
        int currentIndex = 0;
        for (auto& vertex : baseGrid)
        {
            vertices.push_back(vertex);
            indices.push_back(currentIndex);
            currentIndex++;
        }
    }
    
    //regenerate vertices based on selected agent
//    if (selectedAgent!=nullptr)
//    {
    //        auto agent = selectedAgent;
    switch (CurrentRenderMode)
    {
        case RenderMode::POINT_ALL:
            for (auto& agent : agents)
                for (auto& node : agent->nodes)
                {
                    unsigned int currentIndex = vertices.size();
                    vertices.push_back(Vertex(node.Position,agent->color));
                    indices.push_back(currentIndex);
                }
            break;
        case RenderMode::WIRE_ALL:
            for (auto& agent : agents)
            {
                unsigned int currentIndex = vertices.size();
                for (auto& node : agent->nodes)
                {
                    vertices.push_back(Vertex(node.Position, agent->color));
                }
                for (auto& spring : agent->springs)
                {
                    indices.push_back(currentIndex + spring.obj1);
                    indices.push_back(currentIndex + spring.obj2);
                }
            }
            break;
        case RenderMode::POINT_ONE:
            for (auto& node : agents[selectedAgent]->nodes)
            {
                unsigned int currentIndex = vertices.size();
                vertices.push_back(Vertex(node.Position,agents[selectedAgent]->color));
                indices.push_back(currentIndex);
            }
            break;
        case RenderMode::WIRE_ONE:
            unsigned int currentIndex = vertices.size();
            for (auto& node : agents[selectedAgent]->nodes)
            {
                vertices.push_back(Vertex(node.Position, agents[selectedAgent]->color));
            }
            for (auto& spring : agents[selectedAgent]->springs)
            {
                indices.push_back(spring.obj1 + currentIndex);
                indices.push_back(spring.obj2 + currentIndex);
            }
            break;
    }
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);
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
            case RenderMode::POINT_ONE:
            case RenderMode::POINT_ALL:
                polyType=GL_POINTS;
                break;
            case RenderMode::WIRE_ONE:
            case RenderMode::WIRE_ALL:
                polyType=GL_LINES;
                break;
        }
        glDrawElements(polyType, indices.size(), GL_UNSIGNED_INT, (void*)0);
    }
    
    glBindVertexArray(0);
    
}


void EvolutionSystem::updateAgent(SoftBodyAgent *agent)
{
    int rep = accelerate ? generationLength : 1;
    for (int i =0;i<rep;++i)
    {
        //apply external forces
        for (auto& node : agent->nodes)
        {
            //            node.ApplyForce(glm::vec3(0,0,-1));
            //            if (node->Position.x > 0.25) node->ApplyForce(glm::vec3(0,1,0));
            node.ApplyForce(glm::vec3(0,0,-1));
            if (node.Position.z < 0)
            {
                float normalForce =-node.Position.z*10000 - node.NetForce.z;
                const float k_friction = 0.1;
                node.ApplyForce(glm::vec3(0,0,normalForce));
                if (node.Velocity.x!=0 && node.Velocity.y!=0)
                    node.ApplyForce(-normalForce * k_friction * glm::normalize(glm::vec3(node.Velocity.x,node.Velocity.y,0)));
            }
        }
        //    selectedAgent->nodes[0]->ApplyForce(glm::vec3(20,0,0));
        agent->Update(TIME_STEP, currentTime*TIME_STEP*100);
    }
}

void EvolutionSystem::Update()
{
    if (!accelerate)
    {
        for (int i = 0; i<(accelerate ? 1 : 50); ++i)
        {
            for (auto& agent : agents)
                updateAgent(agent);
            currentTime++;
            
            if (currentTime%generationLength==0) nextGeneration();
        }
    }
    else
    {
        std::vector<std::thread> threads;
        threads.reserve(agents.size());
        for (SoftBodyAgent* agent : agents) threads.push_back(std::thread(&EvolutionSystem::updateAgent, this, agent));
        
        currentTime+=generationLength;
        for (auto& thread : threads) thread.join();
        nextGeneration();
        
    }
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
        int index =(int)distribution(RandomUtils::rand);
        if (index > agents.size()-1) index = agents.size()-1;
        newAgents[i] = new SoftBodyAgent(*agents[index]);
    }

    for (auto& agent : agents) delete agent;
    agents = newAgents;
}

void EvolutionSystem::configurePerformanceFunctions()
{
    performanceFunctions.push_back(std::function<float(SoftBodyAgent&)>([this](SoftBodyAgent& agent) {
        return pow(0.1f + agent.TotalDistance / generationLength,2);
    }));performanceFunctions.push_back(std::function<float(SoftBodyAgent&)>([this](SoftBodyAgent& agent) {
        return pow(0.1f + agent.TotalMinimumHeight / generationLength,2);
    }));
}
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
#include "glm/gtx/rotate_vector.hpp"
#include <iostream>
#include <chrono>
EvolutionSystem::EvolutionSystem(std::string _outputFileLocation, ConfigurationManager& configManager) :
NUM_AGENTS(configManager.GetItem<int>("NumAgents")),
TIME_STEP(0.0005),
CurrentRenderMode(RenderMode::POINT_ALL),
generationLength(15./TIME_STEP),
outputFileLocation(_outputFileLocation),
currentFunction(0),
accelerate(false),
selectedAgent(0),
GRAVITATIONAL_ACCELERATION(-3.f),
playbackRate(50),
walls{Wall(glm::vec3(0,-0.5f,-0.7),glm::rotate(glm::vec3(0,0,1), 20.0f, glm::vec3(1,0,0)),1.9f)}
//Wall(glm::vec3(0,0.5f,-0.3),glm::rotate(glm::vec3(0,0,1), -10.0f, glm::vec3(1,0,0)),0.9f)}
{
    SoftBodyAgent::INITIAL_CUBE_WIDTH = configManager.GetItem<int>("AgentCubeWidth");
    SoftBodyAgent::NODE_SPACING = configManager.GetItem<float>("AgentNodeSpacing");
    
    configurePerformanceFunctions();
    for (int i = 0; i<NUM_AGENTS;++i)
        agents.push_back(new SoftBodyAgent(glm::vec3(0,0,0.01), glm::vec3(RandomUtils::UniformFloat(),RandomUtils::UniformFloat(),RandomUtils::UniformFloat())));
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


void EvolutionSystem::updateBuffers()
{
    vertices.clear();
    indices.clear();
    for (auto& wall:walls) wall.AppendVertices(vertices, indices);
    
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
                    vertices.push_back(Vertex(node.Position,agent->GetNodeColor(node)));
                    indices.push_back(currentIndex);
                }
            break;
        case RenderMode::WIRE_ALL:
            for (auto& agent : agents)
            {
                unsigned int currentIndex = vertices.size();
                for (auto& node : agent->nodes)
                {
                    vertices.push_back(Vertex(node.Position, agent->GetNodeColor(node)));
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
                vertices.push_back(Vertex(node.Position,agents[selectedAgent]->GetNodeColor(node)));
                indices.push_back(currentIndex);
            }
            break;
        case RenderMode::WIRE_ONE:
            unsigned int currentIndex = vertices.size();
            for (auto& node : agents[selectedAgent]->nodes)
            {
                vertices.push_back(Vertex(node.Position, agents[selectedAgent]->GetNodeColor(node)));
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
            node.ApplyForce(glm::vec3(0,0,GRAVITATIONAL_ACCELERATION*node.Mass));
            if (node.Velocity!=glm::vec3())
                node.ApplyForce(-DRAG_COEFFICIENT*glm::normalize(node.Velocity));
//            if (node.Position.z < 0)
//            {
//                float normalForce =-node.Position.z*10000 - node.NetForce.z;
//                const float k_friction = 0.9;
//                node.ApplyForce(glm::vec3(0,0,normalForce));
//                if (node.Velocity.x!=0 && node.Velocity.y!=0)
//                    node.ApplyForce(-normalForce * k_friction * glm::normalize(glm::vec3(node.Velocity.x,node.Velocity.y,0)));
//            }
            for (Wall& wall : walls) wall.ApplyForce(dynamic_cast<PhysicsObject*>(&node));
        }
        //    selectedAgent->nodes[0]->ApplyForce(glm::vec3(20,0,0));
        agent->Update(TIME_STEP, currentTime*TIME_STEP*100);
    }
}
void EvolutionSystem::Update()
{
    if (!accelerate)
    {
        for (int i = 0; i<(accelerate ? 1 : playbackRate); ++i)
        {
            for (auto& agent : agents)
                updateAgent(agent);
            currentTime++;
            
            if (currentTime%generationLength==0) nextGeneration();
        }
    }
    else
    {
//        auto t = std::chrono::high_resolution_clock::now();
        std::vector<std::thread> threads;
        threads.reserve(agents.size());
        for (SoftBodyAgent* agent : agents) threads.push_back(std::thread(&EvolutionSystem::updateAgent, this, agent));
        
        currentTime+=generationLength;
        for (auto& thread : threads) thread.join();
        
//        std::cout << "Time passed: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count() << " ms" << std::endl;
        nextGeneration();
        
        
    }
}
void EvolutionSystem::nextGeneration()
{
    std::ofstream stream(outputFileLocation, std::ios::out | std::ios::app);
    std::vector<float> intervals(agents.size(), 1.0);
    std::vector<float> probabilities(agents.size());
    float average=0;
    float averageEnergy=0;
//    float min=0;
    for (int i = 0; i<agents.size();++i)
    {
        probabilities[i] = performanceFunctions[currentFunction](*agents[i]);
        averageEnergy+=agents[i]->TotalEnergy/generationLength;
//        if (probabilities[i]<min || min==0) min=probabilities[i];
    }
    averageEnergy/=agents.size();
    for(auto& p : probabilities) {
//        p-=min;
        average+=p;
    }
//    for (auto& p :probabilities) p/=average;
    average/=probabilities.size();
    stream << currentTime/generationLength << "," << average << "," << averageEnergy << std::endl;
    std::cout << currentTime/generationLength << "," << average << "," << averageEnergy << std::endl;
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
        return pow(agent.TotalDistance / agent.TotalEnergy,2)/(agent.Size+0.001)*generationLength;
    }));performanceFunctions.push_back(std::function<float(SoftBodyAgent&)>([this](SoftBodyAgent& agent) {
        return (agent.TotalMinimumHeight / agent.TotalEnergy)/(agent.Size*generationLength+0.001);
    }));
}
//
//  EvolutionSystem.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "EvolutionSystem.h"

EvolutionSystem::EvolutionSystem() :
agents{SoftBodyAgent(glm::vec3(0,0,1))},
TIME_STEP(0.0001),
CurrentRenderMode(RenderMode::POINT)
{
    selectedAgent=&agents[0];
    generateBuffers();
    updateBuffers();
}
static const EvolutionSystem::Vertex g_quad_vertex_buffer_data[] = {
    EvolutionSystem::Vertex(-1.0f, -1.0f, 0.0f),
    EvolutionSystem::Vertex(1.0f, -1.0f, 0.0f),
    EvolutionSystem::Vertex(-1.0f,  1.0f, 0.0f),
    EvolutionSystem::Vertex(-1.0f,  1.0f, 0.0f),
    EvolutionSystem::Vertex(1.0f, -1.0f, 0.0f),
    EvolutionSystem::Vertex(1.0f,  1.0f, 0.0f),
};
#include <iostream>
void EvolutionSystem::generateBuffers()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);
}

void EvolutionSystem::updateBuffers()
{
    vertices.clear();
    for (auto& vertex : g_quad_vertex_buffer_data) vertices.push_back(vertex);
    
    //regenerate vertices based on selected agent
    if (selectedAgent!=nullptr)
    {
        switch (CurrentRenderMode)
        {
            case RenderMode::POINT:
                for (auto& node : selectedAgent->nodes)
                {
                    vertices.push_back(node->Position);
                }
                break;
            case RenderMode::WIRE:
                for (auto& spring : selectedAgent->springs)
                {
                    vertices.push_back(spring.obj1->Position);
                    vertices.push_back(spring.obj2->Position);
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
        for (auto& node : agent.nodes)
        {
//            node.ApplyForce(glm::vec3(0,0,-1));
            if (node->Position.x > 0.25) node->ApplyForce(glm::vec3(0,1,0));
            node->ApplyForce(glm::vec3(0,0,-1));
            if (node->Position.z < 0)
            {
                float normalForce =-node->Position.z*10000 - node->NetForce.z;
                const float k_friction = 0.3;
                node->ApplyForce(glm::vec3(0,0,normalForce));
                if (node->Velocity.x!=0 && node->Velocity.y!=0)
                node->ApplyForce(-normalForce * k_friction * glm::normalize(glm::vec3(node->Velocity.x,node->Velocity.y,0)));
            }
        }
    }
//    selectedAgent->nodes[0]->ApplyForce(glm::vec3(20,0,0));
    for (auto& agent : agents)
        agent.Update(TIME_STEP);
    updateBuffers();
}
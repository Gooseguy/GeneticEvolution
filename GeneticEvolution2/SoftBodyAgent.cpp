//
//  SoftBodyAgent.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "SoftBodyAgent.h"
#include <OpenGL/gl3.h>
#include "glm/gtx/euler_angles.hpp"
#include "RandomUtils.h"
#include "Spring.h"
const float SoftBodyAgent::NODE_SPACING = 0.05f;

SoftBodyAgent::SoftBodyAgent(glm::vec3 pos, glm::vec3 _color) : TotalMinimumHeight(0),color(_color)
{
    glm::mat3 rotMat =glm::mat3();//glm::mat3(glm::eulerAngleXZ(30*3.141f/180,0.f));
    for (int i = 0; i<INITIAL_CUBE_WIDTH;++i)
        for (int j = 0; j<INITIAL_CUBE_WIDTH;++j)
            for (int k = 0; k<INITIAL_CUBE_WIDTH;++k)
            {
                nodes.push_back(SoftBodyNode(pos + rotMat * glm::vec3(i*NODE_SPACING,j*NODE_SPACING,k*NODE_SPACING)));
            }
    for (auto& node : nodes) initialPositions.push_back(node.Position);
    addSpringDisplacement(1, 0, 0);
    addSpringDisplacement(0, 1, 0);
    addSpringDisplacement(0, 0, 1);
    addSpringDisplacement(1, 0, 1);
    addSpringDisplacement(1, 1, 0);
    addSpringDisplacement(0, 1, 1);
    addSpringDisplacement(-1, 0, 0);
    addSpringDisplacement(0, -1, 0);
    addSpringDisplacement(0, 0, -1);
    addSpringDisplacement(1, 0, -1);
    addSpringDisplacement(1, -1, 0);
    addSpringDisplacement(0, 1, -1);
    addSpringDisplacement(-1, 0, 1);
    addSpringDisplacement(-1, 1, 0);
    addSpringDisplacement(0, -1, 1);
    addSpringDisplacement(-1, 0, -1);
    addSpringDisplacement(-1, -1, 0);
    addSpringDisplacement(0, -1, -1);
    
    addSpringDisplacement(1, 1, 1);
    addSpringDisplacement(1, 1, -1);
    addSpringDisplacement(1, -1, 1);
    addSpringDisplacement(-1, 1, 1);
    addSpringDisplacement(-1, -1, 1);
    addSpringDisplacement(-1, 1, -1);
    addSpringDisplacement(1, -1, -1);
    addSpringDisplacement(-1, -1, -1);
    
}

SoftBodyAgent::SoftBodyAgent(const SoftBodyAgent& agent) :
initialPositions(agent.initialPositions),
TotalMinimumHeight(0),
color(agent.color)
{
    for (int i = 0; i<agent.nodes.size();++i)
    {
        nodes.push_back(SoftBodyNode(agent.initialPositions[i]));
    }
    for (auto& spring : agent.springs)
        springs.push_back(Spring(spring));
    Mutate();
    
}

#include<iostream>
SoftBodyAgent::~SoftBodyAgent()
{
//    for (auto& node : nodes) { delete node; node=nullptr; }
//    std::cout << "Destructed." << std::endl;
}

void SoftBodyAgent::Update(float timeStep, int currentTime)
{
    for (auto& spring : springs)
        spring.ApplyForces(currentTime, nodes);
    for (auto& node : nodes)
        node.Update(timeStep);
    SoftBodyNode* lowestNode = nullptr;
    glm::vec3 averageDisp=glm::vec3();
//    float averageHeight = 0;
    for (auto& node : nodes)
    {
        if (lowestNode==nullptr) lowestNode = &node;
        else if (node.Position.z < lowestNode->Position.z) lowestNode=&node;
        averageDisp+=node.Position;
    }
    averageDisp/=nodes.size();
    TotalMinimumHeight+=lowestNode->Position.z;
    TotalDistance+=glm::length(averageDisp);//lowestNode->Position.z;
}

void SoftBodyAgent::addSpring(std::size_t node1, std::size_t node2)
{
    std::size_t idx = springs.size();
    springs.push_back(Spring(node1, node2, nodes));
    nodes[node1].AddSpring(idx);
    nodes[node2].AddSpring(idx);
}

void SoftBodyAgent::RemoveNode(std::size_t node)
{
    for (int i = 0; i<nodes[node].GetSpringsUsed();++i)
    {
        springs.erase(springs.begin() + nodes[node].springs[i]);
    }
}

void SoftBodyAgent::Mutate()
{
    const float extensionAmountVariance = 0.1;
    const float extensionPeriodVariance = 0.1;
    const float extensionLengthVariance = 0.9;
    const float extensionOffsetVariance = 0.9;
//    for (auto& pos : initialPositions)
//    {
//        pos.x+=RandomUtils::Instance.Normal<float>(0, 0.01);
//        pos.y+=RandomUtils::Instance.Normal<float>(0, 0.01);
//        pos.z+=RandomUtils::Instance.Normal<float>(0, 0.01);
//        if (pos.z < 0.01) pos.z = 0.01;
////
//    }
//    for (int i = 0; i<nodes.size();++i)
//    {
//        if (RandomUtils::UniformFloat() < 0.001)
//            RemoveNode(i);
//    }
    
    for (Spring& spring : springs)
    {
        spring.ExtensionAmount+=RandomUtils::Normal<float>(0, extensionAmountVariance);
        float extensionAmountMax=spring.EquilibriumDist*2;
        if (spring.ExtensionAmount>extensionAmountMax)spring.ExtensionAmount=extensionAmountMax;
        else if (spring.ExtensionAmount<-extensionAmountMax)spring.ExtensionAmount=-extensionAmountMax;
//        spring.EquilibriumDist+=RandomUtils::Instance.Normal<float>(0, 0.1);
        spring.ExtensionLength+=RandomUtils::Normal<float>(0, extensionLengthVariance);
        spring.ExtensionOffset+=RandomUtils::Normal<float>(0, extensionOffsetVariance);
        spring.ExtensionLength%=spring.ExtensionPeriod;
        spring.ExtensionOffset%=spring.ExtensionPeriod;
        color.x+=RandomUtils::Normal<float>(0.f, 0.05f);
        color.x = std::abs(color.x);
        color.y+=RandomUtils::Normal<float>(0.f, 0.05f);
        color.x = std::abs(color.y);
        color.z+=RandomUtils::Normal<float>(0.f, 0.05f);
        color.x = std::abs(color.z);
        color = glm::normalize(color);
//        spring.ExtensionPeriod+=RandomUtils::Instance.Normal<float>(0, extensionPeriodVariance);
//        if (spring.ExtensionPeriod<1)spring.ExtensionPeriod=1;
    }
}
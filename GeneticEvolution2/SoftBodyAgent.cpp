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
#include <fstream>
#include <sstream>
float SoftBodyAgent::NODE_SPACING = 0.05f;
int SoftBodyAgent::INITIAL_CUBE_WIDTH=3;
float SoftBodyAgent::ExtensionAmountVariance = 0.05f;
float SoftBodyAgent::ExtensionPeriodVariance = 0.05f;
float SoftBodyAgent::ExtensionLengthVariance = 0.05f;
float SoftBodyAgent::ExtensionOffsetVariance = 0.05f;
float SoftBodyAgent::ExtensionAmountMaxMult = 4.f;
float SoftBodyAgent::GeometryMutationFactor = 0.001f;
float SoftBodyAgent::RenderColorMult=0.00001f;
float SoftBodyAgent::AddNodeProbability=0.01f;
float SoftBodyAgent::RemoveNodeProbability=0.0f;

SoftBodyAgent::SoftBodyAgent(glm::vec3 pos, glm::vec3 _color) : TotalMinimumHeight(0),color(_color), Size(0)
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
    getSize();
    
}

SoftBodyAgent::SoftBodyAgent(const SoftBodyAgent& agent, bool mutate) :
initialPositions(agent.initialPositions),
TotalMinimumHeight(0),
color(agent.color)
{
    if (mutate) mutateGeometry();
    for (int i = 0; i<agent.nodes.size();++i)
    {
        nodes.push_back(SoftBodyNode(initialPositions[i]));
    }
    for (auto& spring : agent.springs)
        springs.push_back(Spring(spring, nodes));
    if (mutate) Mutate();
    getSize();
    
}

#include<iostream>
SoftBodyAgent::~SoftBodyAgent()
{
//    for (auto& node : nodes) { delete node; node=nullptr; }
//    std::cout << "Destructed." << std::endl;
}

void SoftBodyAgent::getSize()
{
    StartingPos=glm::vec3();
    for (auto& node : nodes)
        StartingPos+=node.Position;
    StartingPos/=nodes.size();
}

void SoftBodyAgent::Update(float timeStep, int currentTime)
{
    float potentialEnergy=0;
    for (auto& spring : springs)
    {
        spring.ApplyForces(currentTime, nodes);
        potentialEnergy+=spring.GetEnergy(nodes,currentTime);
    }
    float kineticEnergy=0;
    for (auto& node : nodes)
    {
        node.Update(timeStep);
        kineticEnergy+=node.GetKineticEnergy();
    }
    kineticEnergy/=nodes.size();
    potentialEnergy/=springs.size();
//    assert(kineticEnergy+potentialEnergy>0);
    TotalEnergy+=kineticEnergy;
    TotalEnergy+=potentialEnergy;
//    assert(!std::isnan(TotalEnergy));
    SoftBodyNode* lowestNode = nullptr;
    glm::vec3 averageDisp=glm::vec3();
//    float averageHeight = 0;
    float ndist=0;
    for (auto& node : nodes)
    {
        if (lowestNode==nullptr) lowestNode = &node;
        else if (node.Position.z < lowestNode->Position.z) lowestNode=&node;
//        ndist+=glm::length(node.Position-StartingPos);
        averageDisp+=node.Position;
    }
    averageDisp/=nodes.size();
    ndist/=nodes.size();
    float newSize=0;
    for (auto& node:nodes)
    {
        float dist = glm::length(node.Position-StartingPos);
        newSize += dist;
    }
    newSize/=nodes.size();
    Size+=newSize;
    TotalMinimumHeight+=lowestNode->Position.z;
    TotalDistance+=glm::length(averageDisp-StartingPos);
    
    
//    if (std::isnan(TotalDistance))
//        std::cout << averageDisp.x << ","<< averageDisp.y << ","<< averageDisp.z << "," << nodes.size() << std::endl;
}

void SoftBodyAgent::addSpring(std::size_t node1, std::size_t node2)
{
    std::size_t idx = springs.size();
    springs.push_back(Spring(node1, node2, nodes));
    nodes[node1].AddSpring(idx);
    nodes[node2].AddSpring(idx);
}

void SoftBodyAgent::removeSpring(size_t i)
{
    nodes[springs[i].obj1].springsUsed--;
    nodes[springs[i].obj2].springsUsed--;
    for (auto& node : nodes)
    {
        for (int j = 0; j< node.GetSpringsUsed();++j)
        {
            if (node.springs[j]>i)j--;
        }
    }
    springs.erase(springs.begin() + i);
}

void SoftBodyAgent::RemoveNode(std::size_t node)
{
    if (nodes.size() <= 1) return;
    for (size_t i = 0; i<nodes[node].GetSpringsUsed();++i)
    {
        removeSpring(i);
    }
    nodes.erase(nodes.begin()+node);
    for (auto& spring : springs)
    {
        if (spring.obj1>node) spring.obj1--;
        if (spring.obj2>node) spring.obj2--;
    }
    initialPositions.erase(initialPositions.begin() + node);
}
//TODO: Fix this function.  It can lead to access violations. 
void SoftBodyAgent::AddNode(SoftBodyNode node)
{
    const int numConnections = 4;
    std::vector<std::pair<size_t, SoftBodyNode>> nodesSorted;
    nodesSorted.reserve(nodes.size());
    for (int i =0; i<nodes.size();++i) nodesSorted.push_back(std::pair<size_t, SoftBodyNode>(i, nodes[i]));
    std::sort(nodesSorted.begin(), nodesSorted.end(), [&node](std::pair<size_t, SoftBodyNode>& node1, std::pair<size_t, SoftBodyNode>& node2)->bool {
        return glm::length2(node1.second.Position-node.Position)<glm::length2(node2.second.Position-node.Position);
    });
    size_t nodeIndex=nodes.size();
    nodes.push_back(node);
    for (int i = 0; i<numConnections;++i)
    {
        springs.push_back(Spring(nodesSorted[i].first, nodeIndex,nodes));
    }
    initialPositions.push_back(node.Position);
}

void SoftBodyAgent::mutateGeometry()
{
    for (auto& pos : initialPositions)
    {
        pos.x+=RandomUtils::Normal(0.0f, GeometryMutationFactor);
        pos.y+=RandomUtils::Normal(0.0f, GeometryMutationFactor);
        pos.z+=RandomUtils::Normal(0.0f, GeometryMutationFactor);
        if (pos.z<0.01f)pos.z=0.001f;
    }
}

void SoftBodyAgent::Mutate()
{
//    for (auto& pos : initialPositions)
//    {
//        pos.x+=RandomUtils::Instance.Normal<float>(0, 0.01);
//        pos.y+=RandomUtils::Instance.Normal<float>(0, 0.01);
//        pos.z+=RandomUtils::Instance.Normal<float>(0, 0.01);
//        if (pos.z < 0.01) pos.z = 0.01;
////
//    }
    for (int i = 0; i<nodes.size();++i)
    {
        if (RandomUtils::UniformFloat() < RemoveNodeProbability)
            RemoveNode(i);
        if (RandomUtils::UniformFloat() < AddNodeProbability)
        {
            const float maxDist = 0.05;
            SoftBodyNode node =SoftBodyNode(nodes[nodes.size() * RandomUtils::UniformFloat()].Position + glm::vec3(RandomUtils::Uniform(-maxDist,maxDist),RandomUtils::Uniform(-maxDist,maxDist),RandomUtils::Uniform(-maxDist,maxDist)));
            if (node.Position.z<0.001) node.Position.z=0.001;
            AddNode(node);
        }
    }
    
    
    for (Spring& spring : springs)
    {
        spring.ExtensionAmount+=RandomUtils::Normal<float>(0, ExtensionAmountVariance);
        float extensionAmountMax=spring.EquilibriumDist*ExtensionAmountMaxMult;
        if (spring.ExtensionAmount>extensionAmountMax) spring.ExtensionAmount=extensionAmountMax;
        else if (spring.ExtensionAmount<-extensionAmountMax) spring.ExtensionAmount=-extensionAmountMax;
//        spring.EquilibriumDist+=RandomUtils::Instance.Normal<float>(0, 0.1);
        spring.ExtensionLength+=RandomUtils::Normal<float>(0, ExtensionLengthVariance);
        spring.ExtensionOffset+=RandomUtils::Normal<float>(0, ExtensionOffsetVariance);
        spring.ExtensionLength%=spring.ExtensionPeriod;
        spring.ExtensionOffset%=spring.ExtensionPeriod;
        color.x+=RandomUtils::Normal<float>(0.f, 0.05f);
        color.x = std::abs(color.x);
        color.y+=RandomUtils::Normal<float>(0.f, 0.05f);
        color.x = std::abs(color.y);
        color.z+=RandomUtils::Normal<float>(0.f, 0.05f);
        color.x = std::abs(color.z);
        color = glm::normalize(color);
        spring.ExtensionPeriod+=RandomUtils::Normal<float>(0, ExtensionPeriodVariance);
        if (spring.ExtensionPeriod<1)spring.ExtensionPeriod=1;
    }
}

void SoftBodyAgent::SaveToFile(const std::string &fileName)
{
    std::ofstream stream(fileName, std::ios::out);
    for (auto& vec : initialPositions)
        stream << vec.x << "," << vec.y << "," << vec.z << "," << std::endl;
    stream << std::endl;
    stream << color.x << "," << color.y << "," << color.z << std::endl;
    stream << std::endl;
    for (auto& spring : springs)
        stream << spring.obj1 << "," << spring.obj2 << "," << spring.EquilibriumDist << "," << spring.ExtensionAmount << "," << spring.ExtensionLength << "," << spring.ExtensionOffset << "," << spring.ExtensionPeriod << std::endl;
    
}

SoftBodyAgent::SoftBodyAgent(const std::string& fileName)
{
    std::ifstream stream(fileName, std::ios::in);
    std::string line;
    while (std::getline(stream, line))
    {
        if (line=="") break;
        std::stringstream sstream(line, std::ios::in);
        
        std::string x,y,z;
        std::getline(sstream, x, ',');
        std::getline(sstream, y, ',');
        std::getline(sstream, z, ',');
        glm::vec3 pos(atof(x.c_str()), atof(y.c_str()), atof(z.c_str()));
        initialPositions.push_back(pos);
    }
    std::getline(stream, line);
    {
        std::getline(stream, line);
        std::stringstream sstream(line, std::ios::in);
        
        std::string x,y,z;
        std::getline(sstream, x, ',');
        std::getline(sstream, y, ',');
        std::getline(sstream, z, ',');
        color = glm::vec3(atof(x.c_str()), atof(y.c_str()), atof(z.c_str()));
    }
    for (auto& pos : initialPositions) nodes.push_back(SoftBodyNode(pos));
    
    while (std::getline(stream, line))
    {
        if (line=="") break;
        
        std::stringstream sstream(line, std::ios::in);
        
        std::string obj1, obj2, eqdist, examount, exlen, exoff, exper;
        std::getline(sstream, obj1, ',');
        std::getline(sstream, obj2, ',');
        std::getline(sstream, eqdist, ',');
        std::getline(sstream, examount, ',');
        std::getline(sstream, exlen, ',');
        std::getline(sstream, exoff, ',');
        std::getline(sstream, exper, ',');
        
        Spring spring(atoi(obj1.c_str()), atoi(obj2.c_str()), nodes);
        spring.EquilibriumDist = atof(eqdist.c_str());
        spring.ExtensionAmount = atof(examount.c_str());
        spring.ExtensionLength = atof(exlen.c_str());
        spring.ExtensionOffset = atof(exoff.c_str());
        spring.ExtensionPeriod = atof(exper.c_str());
        springs.push_back(spring);
    }
    getSize();
}
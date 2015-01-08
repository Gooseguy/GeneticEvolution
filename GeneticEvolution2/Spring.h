//
//  Spring.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include "PhysicsObject.h"
#include "SoftBodyNode.h"
#include <vector>
class Spring
{
public:
    Spring (std::size_t index1, std::size_t index2, std::vector<SoftBodyNode>& objects);
    Spring (std::size_t o1,std::size_t o2, std::vector<SoftBodyNode>& objects, float equilibriumDist);
    Spring (const Spring& spring, std::vector<SoftBodyNode>& objects);
    std::size_t obj1;
    std::size_t obj2;
    void ApplyForces(float timeStep, int currentTime, std::vector<SoftBodyNode>& objects);
    float EquilibriumDist;
    static float DefaultSpringConstant;
    float SpringConstant;
    float ExtensionAmount;
    int ExtensionPeriod;
    int ExtensionLength;
    int ExtensionOffset;
    inline bool IsExtending(int currentTime);
    inline float GetEnergy(std::vector<SoftBodyNode>& objects,int currentTime);
private:
    
};

bool Spring::IsExtending(int currentTime)
{
    return (currentTime+ExtensionOffset)%ExtensionPeriod < ExtensionLength;
}

float Spring::GetEnergy(std::vector<SoftBodyNode>& objects,int currentTime)
{
    float diff =glm::length(objects[obj1].Position-objects[obj2].Position) - (EquilibriumDist + (IsExtending(currentTime) ? ExtensionAmount : 0));
    assert(!std::isnan(diff));
    return 0.5f * SpringConstant * diff * diff;
}
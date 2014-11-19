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
    std::size_t obj1;
    std::size_t obj2;
    void ApplyForces(int currentTime, std::vector<SoftBodyNode>& objects);
    float EquilibriumDist;
    float SpringConstant;
    float ExtensionAmount;
    int ExtensionPeriod;
    int ExtensionLength;
    int ExtensionOffset;
private:
    
};
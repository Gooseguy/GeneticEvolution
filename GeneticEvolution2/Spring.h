//
//  Spring.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include "PhysicsObject.h"
class Spring
{
public:
    Spring (PhysicsObject* o1, PhysicsObject* o2);
    Spring (PhysicsObject* o1, PhysicsObject* o2, float equilibriumDist);
    PhysicsObject* obj1;
    PhysicsObject* obj2;
    void ApplyForces();
    float EquilibriumDist;
    float SpringConstant;
private:
    
};
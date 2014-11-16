//
//  Spring.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "Spring.h"

Spring::Spring(PhysicsObject* o1, PhysicsObject* o2) : obj1(o1), obj2(o2), EquilibriumDist(glm::length(o1->Position-o2->Position)), SpringConstant(200)
{
    
}
Spring::Spring(PhysicsObject* o1, PhysicsObject* o2, float equilibriumDist) : obj1(o1), obj2(o2), EquilibriumDist(equilibriumDist), SpringConstant(200)
{
    
}
#include <iostream>
void Spring::ApplyForces()
{
    glm::vec3 disp = obj1->Position - obj2->Position;
    glm::vec3 disp2 = obj1->Position + obj1->Velocity - obj2->Position - obj2->Velocity;
    float dist = glm::length(disp);
    float dist2 = glm::length(disp2);
    float diff = dist-EquilibriumDist;
    float diff2 = dist2-EquilibriumDist;
    if (dist < 50 && dist>0.01 && dist2 < 50 && dist2 > 0.01)
    {
        obj1->ApplyForce(-(diff* SpringConstant) * glm::normalize(disp),-(diff2* SpringConstant) * glm::normalize(disp2));
        obj2->ApplyForce((diff* SpringConstant) * glm::normalize(disp),(diff2* SpringConstant) * glm::normalize(disp2));
    }
}
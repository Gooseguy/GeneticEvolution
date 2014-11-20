//
//  Spring.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "Spring.h"
#include "RandomUtils.h"

Spring::Spring(std::size_t o1,std::size_t o2, std::vector<SoftBodyNode>& objects) : Spring(o1,o2,objects,glm::length(objects[o1].Position-objects[o2].Position))
{
    
}
Spring::Spring(std::size_t o1,std::size_t o2, std::vector<SoftBodyNode>& objects, float equilibriumDist) : obj1(o1), obj2(o2), EquilibriumDist(equilibriumDist), SpringConstant(100), ExtensionAmount(RandomUtils::Normal<float>(0, 0.05f)*0.5f), ExtensionPeriod(50), ExtensionOffset(5*RandomUtils::UniformFloat()), ExtensionLength(RandomUtils::UniformFloat() * ExtensionPeriod)
{
    
}
void Spring::ApplyForces(int currentTime, std::vector<SoftBodyNode>& objects)
{
    PhysicsObject* o1 = &objects[obj1];
    PhysicsObject* o2 = &objects[obj2];
    glm::vec3 disp = o1->Position - o2->Position;
    glm::vec3 disp2 = o1->Position + o1->Velocity - o2->Position - o2->Velocity;
    float dist = glm::length(disp);
    float dist2 = glm::length(disp2);
    float diff = dist-EquilibriumDist;
    float diff2 = dist2-EquilibriumDist;
    if ((currentTime+ExtensionOffset)%ExtensionPeriod < ExtensionLength) {
        diff-=ExtensionAmount;
        diff2-=ExtensionAmount;
    }
    if (dist < 50 && dist>0.01 && dist2 < 50 && dist2 > 0.01)
    {
        o1->ApplyForce(-(diff* SpringConstant) * glm::normalize(disp),-(diff2* SpringConstant) * glm::normalize(disp2));
        o2->ApplyForce((diff* SpringConstant) * glm::normalize(disp),(diff2* SpringConstant) * glm::normalize(disp2));
    }
    
}
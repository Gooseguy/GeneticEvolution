//
//  Spring.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "Spring.h"
#include "RandomUtils.h"

float Spring::DefaultSpringConstant = 125.f;


Spring::Spring(std::size_t o1,std::size_t o2, std::vector<SoftBodyNode>& objects) : Spring(o1,o2,objects,glm::length(objects[o1].Position-objects[o2].Position))
{
    
}
Spring::Spring(std::size_t o1,std::size_t o2, std::vector<SoftBodyNode>& objects, float equilibriumDist) : obj1(o1), obj2(o2), EquilibriumDist(equilibriumDist), SpringConstant(DefaultSpringConstant), ExtensionAmount(RandomUtils::Normal<float>(0, 0.0005f)*0.5f), ExtensionPeriod(50), ExtensionOffset(5*RandomUtils::UniformFloat()), ExtensionLength(RandomUtils::UniformFloat() * ExtensionPeriod)
{
    
}

Spring::Spring(const Spring& spring, std::vector<SoftBodyNode>& objects) : Spring(spring)                                                                                                          {
    EquilibriumDist = glm::length(objects[spring.obj1].Position-objects[spring.obj2].Position);

}
void Spring::ApplyForces(float timeStep, int currentTime, std::vector<SoftBodyNode>& objects)
{
    SoftBodyNode* o1 = &objects.at(obj1);
    SoftBodyNode* o2 = &objects.at(obj2);
    glm::vec3 disp = o1->Position - o2->Position;
    glm::vec3 disp2 = o1->Position + o1->Velocity - o2->Position - o2->Velocity;
    float dist = glm::length(disp);
    float dist2 = glm::length(disp2);
    float diff = dist-EquilibriumDist;
    float diff2 = dist2-EquilibriumDist;
    if (IsExtending(currentTime)) {
        diff-=ExtensionAmount;
        diff2-=ExtensionAmount;
    }
    if (dist>0.01 && dist2 > 0.01 && std::isfinite(dist) && std::isfinite(dist2))
    {
        o1->ApplyForce(-(diff* SpringConstant) * glm::normalize(disp),-(diff2* SpringConstant) * glm::normalize(disp2));
        o1->TotalStress+=std::abs(diff*SpringConstant);
        o2->ApplyForce((diff* SpringConstant) * glm::normalize(disp),(diff2* SpringConstant) * glm::normalize(disp2));
        o2->TotalStress+=std::abs(diff*SpringConstant);
    }
    
}
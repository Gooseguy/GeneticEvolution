//
//  SoftBodyNode.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include "PhysicsObject.h"
#include <array>
class Spring;
class SoftBodyNode : public PhysicsObject
{
public:
    SoftBodyNode(glm::vec3 position);
    SoftBodyNode(const SoftBodyNode& node);
    SoftBodyNode& operator=(const SoftBodyNode& other);
    void Update(float timeStep) override;
    inline int GetSpringsUsed();
    inline void AddSpring(std::size_t spring);
    std::size_t springsUsed;
    static const int MAX_SPRINGS = 64;
    static float DefaultNodeMass;
    float TotalStress;
    std::array<std::size_t, MAX_SPRINGS> springs;
private:
};

int SoftBodyNode::GetSpringsUsed()
{
    return springsUsed;
}
void SoftBodyNode::AddSpring(std::size_t spring)
{
    springs[springsUsed]=spring;
    springsUsed++;
    if (springsUsed>=MAX_SPRINGS) throw std::out_of_range("Out of range");
    //this will throw an error if the maximum number of springs is exceeded for a single node.
}
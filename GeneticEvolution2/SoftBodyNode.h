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
#include "Spring.h"
class SoftBodyNode : public PhysicsObject
{
public:
    SoftBodyNode(glm::vec3 position);
    void Update(float timeStep) override;
    inline int GetSpringsUsed();
    inline void AddSpring(Spring* spring);
private:
    static const int MAX_SPRINGS = 5;
    std::array<Spring*, MAX_SPRINGS> springs;
};

int SoftBodyNode::GetSpringsUsed()
{
    int i;
    for (i = 0; springs[i]!=nullptr;++i);
    return i;
}
void SoftBodyNode::AddSpring(Spring* spring)
{
    int i = GetSpringsUsed();
    springs[i]=spring;
    //this will throw an error if the maximum number of springs is exceeded for a single node.
}
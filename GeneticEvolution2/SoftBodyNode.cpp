//
//  SoftBodyNode.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "SoftBodyNode.h"
SoftBodyNode::SoftBodyNode(glm::vec3 pos) : PhysicsObject(pos,1)
{
    for (auto& p : springs) p=nullptr;
}


void SoftBodyNode::Update(float timeStep)
{
    PhysicsObject::Update(timeStep);
}
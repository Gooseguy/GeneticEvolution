//
//  SoftBodyNode.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "SoftBodyNode.h"

float SoftBodyNode::DefaultNodeMass = 1.f;

SoftBodyNode::SoftBodyNode(glm::vec3 pos) : PhysicsObject(pos,DefaultNodeMass), springsUsed(0)
{
    
}

SoftBodyNode::SoftBodyNode(const SoftBodyNode& node) : PhysicsObject(node.Position, node.Mass), springsUsed(node.springsUsed), springs(node.springs)
{
}

SoftBodyNode& SoftBodyNode::operator=(const SoftBodyNode &other)
{
    Position=other.Position;
    Velocity=other.Velocity;
//    Mass=other.Mass;
    springsUsed=other.springsUsed;
    springs=other.springs;
    return *this;
}

void SoftBodyNode::Update(float timeStep)
{
    PhysicsObject::Update(timeStep);
}
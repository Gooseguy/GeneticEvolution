//
//  PhysicsObject.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(glm::vec3 pos, float mass) : Position(pos), Mass(mass)
{
    
}
PhysicsObject::PhysicsObject(glm::vec3 pos, glm::vec3 vel, float mass) : Position(pos), Velocity(vel), Mass(mass)
{
    
}

void PhysicsObject::Update(float timeStep)
{
    //perform Velocity Verlet integration
    //see http://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet
    auto currAcceleration = NetForce / Mass;
    auto newPos = Position + Velocity * timeStep + 0.5f * currAcceleration * timeStep * timeStep;
    auto newAcceleration = NextNetForce * (1.f/Mass);
    auto newVelocity = Velocity + 0.5f * timeStep * (currAcceleration + newAcceleration);
    Velocity = newVelocity;
    Position = newPos;
    NetForce=glm::vec3();
    NextNetForce=glm::vec3();
}
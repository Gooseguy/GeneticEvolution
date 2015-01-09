//
//  PhysicsObject.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/13/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

class PhysicsObject
{
public:
    glm::vec3 Position;
    glm::vec3 Velocity;
    glm::vec3 NetForce;
    glm::vec3 NextNetForce;
    const float Mass;
    PhysicsObject(glm::vec3 pos, float mass);
    PhysicsObject(glm::vec3 pos, glm::vec3 vel, float mass);
    virtual void Update(float timeStep);
    
    inline void ApplyForce(glm::vec3 force);
    inline void ApplyForce(glm::vec3 force, glm::vec3 nextForce);
    inline float GetKineticEnergy();
private:
};

void PhysicsObject::ApplyForce(glm::vec3 force)
{
    ApplyForce(force, force);
}
void PhysicsObject::ApplyForce(glm::vec3 force,glm::vec3 nextForce)
{
    NetForce+=force;
    NextNetForce+=nextForce;
    assert(std::isfinite(NetForce.x) || std::isfinite(NetForce.y) || std::isfinite(NetForce.z));
}

float PhysicsObject::GetKineticEnergy()
{
    return 0.5f * Mass * glm::length2(Velocity);
}
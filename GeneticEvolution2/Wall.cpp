//
//  Wall.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/21/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "Wall.h"

const float Wall::FORCE_MULT = 10000.0f;

Wall::Wall(glm::vec3 origin, glm::vec3 normal, float frictionConstant) : Origin(origin), Normal(normal), FrictionConstant(frictionConstant), gridSquareLength(0.05)
{
    constructBaseGrid();
}
void Wall::ApplyForce(PhysicsObject* object)
{
    glm::vec3 normalComponent = Normal * (glm::dot(Normal, object->Position - Origin));
    if (glm::dot(Normal, object->Position - Origin) < 0) // if object is underneath plane
    {
        glm::vec3 normalForce = -normalComponent*FORCE_MULT - Normal*(glm::dot(Normal, object->NetForce));
        float frictionMagnitude = (glm::dot(Normal, object->Position - Origin)) * FORCE_MULT * FrictionConstant;
        glm::vec3 planarVelocity =object->Velocity - Normal*(glm::dot(Normal, object->Velocity));
        glm::vec3 frictionForce=glm::vec3();
        if (planarVelocity!=glm::vec3())
        {
            frictionForce=frictionMagnitude * (glm::normalize(planarVelocity));
        }
        object->ApplyForce(normalForce+frictionForce);
    }
}

void Wall::AppendVertices(std::vector<Vertex> &vertices, std::vector<unsigned int>& indices)
{
    unsigned int currentIndex = vertices.size();
    for (auto& vertex:baseGrid)
    {
        vertices.push_back(vertex);
        indices.push_back(currentIndex);
        currentIndex++;
    }
}

void Wall::constructBaseGrid()
{
    const float radius = 2.f;
    //basis vectors for plane
    glm::vec3 bx = glm::vec3(1,0,0);
    glm::vec3 by = glm::cross(Normal, bx);
    for (float i = -radius; i<=radius+gridSquareLength;i+=gridSquareLength)
    {
        baseGrid.push_back(Vertex(Origin + -std::sqrt(radius*radius-i*i)*bx + i*by,glm::vec3(1.f,0,0)));
        baseGrid.push_back(Vertex(Origin + std::sqrt(radius*radius-i*i)*bx + i*by,glm::vec3(1.f,0,0)));
    }
    for (float i = -radius; i<=radius+gridSquareLength;i+=gridSquareLength)
    {
        baseGrid.push_back(Vertex(Origin + i*bx + -std::sqrt(radius*radius-i*i)*by,glm::vec3(1.f,0,0)));
        baseGrid.push_back(Vertex(Origin + i*bx + std::sqrt(radius*radius-i*i)*by,glm::vec3(1.f,0,0)));
    }
}


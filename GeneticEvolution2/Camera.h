//
//  Camera.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class Camera
{
public:
    Camera(int windowWidth, int windowHeight);
    glm::mat4 GetTransformMatrix();
    glm::mat4 GetProjectionMatrix();
    void Update();
    float FieldOfView;
    const float FAR_PLANE;
    const float NEAR_PLANE;
private:
    
    float rotationX,rotationZ;
    glm::quat rotationQuat;
    glm::vec3 position;
    float aspectRatio;
    glm::quat angleAxis(float angle, glm::vec3 axis);
};
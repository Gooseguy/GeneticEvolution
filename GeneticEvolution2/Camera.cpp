//
//  Camera.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "Camera.h"
#include <SDL2/SDL.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "ConfigurationManager.h"
Camera::Camera(int windowWidth, int windowHeight, ConfigurationManager& configManager) :
FieldOfView(configManager.GetItem<float>("FieldOfView")),
aspectRatio((float)windowWidth/windowHeight),
NEAR_PLANE(0.000001),
FAR_PLANE(100),
rotationQuat(0,0,0, 1),
rotationX(0),
rotationZ(0),
position(1,0.1,-0.4)

{
    
}
glm::mat4 Camera::GetTransformMatrix()
{
    return GetProjectionMatrix() * glm::mat4_cast(rotationQuat) * glm::translate(glm::mat4(), position);
}
glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(
                            FieldOfView,
                            aspectRatio,
                            NEAR_PLANE,  //near clipping plane
                            FAR_PLANE //far clipping plane
                            );
}

void Camera::Update()
{
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    const float rotationSpeed = 0.01;
    rotationX-=rotationSpeed*x;
    rotationZ-=rotationSpeed*y;
    if (rotationZ<0) rotationZ=0;
    if (rotationZ>M_PI) rotationZ=M_PI;
    rotationQuat = angleAxis(rotationZ,(glm::vec3(0,0,1))) * angleAxis(rotationX,(glm::vec3(1,0,0)));
    const float movementSpeed = 0.01;
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_W])
        position+= movementSpeed * glm::vec3(0,0,1) * glm::mat3_cast(rotationQuat);
    if (state[SDL_SCANCODE_S])
        position+= movementSpeed * glm::vec3(0,0,-1) * glm::mat3_cast(rotationQuat);
    if (state[SDL_SCANCODE_A])
        position+= movementSpeed * glm::vec3(1,0,0) * glm::mat3_cast(rotationQuat);
    if (state[SDL_SCANCODE_D])
        position+= movementSpeed * glm::vec3(-1,0,0) * glm::mat3_cast(rotationQuat);
}

glm::quat Camera::angleAxis(float angle, glm::vec3 axis)
{
    return glm::quat(axis.x * std::sin(angle / 2), axis.y * std::sin(angle/2), axis.z * std::sin(angle / 2), std::cos(angle / 2));
}


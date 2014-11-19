//
//  MainGame.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//

#include "MainGame.h"
#include <OpenGL/gl3.h>
#include <stdexcept>
#include "GLManager.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "ResourcePath.hpp"
#include <fstream>

MainGame::MainGame()
{
    //if SDL fails, close program
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL!  " + std::string(SDL_GetError()));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    window = SDL_CreateWindow("Genetic Algorithm", 0, 0, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    std::string fileLoc =resourcePath() + "performance.csv";
    {
        std::ofstream stream(fileLoc, std::ios::out);
    }
    EvolutionSystem evolutionSystem(fileLoc);
    Camera camera(1280, 720);
    
    while (GameState!=GameState::EXIT)
    {
        Update(evolutionSystem);
        camera.Update();
        glManager.Programs[0].SetMatrix4("transformMatrix", glm::value_ptr(camera.GetTransformMatrix()));
        Draw(evolutionSystem);
        SDL_GL_SwapWindow(window);
        HandleEvents(evolutionSystem);
    }
}

void MainGame::Update(EvolutionSystem& evolutionSystem)
{
    for (int i = 0; i<50;++i)
        evolutionSystem.Update();
}

void MainGame::Draw(EvolutionSystem& evolutionSystem)
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    evolutionSystem.Draw();
}

void MainGame::HandleEvents(EvolutionSystem& evolutionSystem)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                GameState=GameState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    GameState=GameState::EXIT;
                    break;
                case SDL_SCANCODE_TAB:
                    if (evolutionSystem.CurrentRenderMode==EvolutionSystem::RenderMode::POINT)
                        evolutionSystem.CurrentRenderMode =EvolutionSystem::RenderMode::WIRE;
                    else evolutionSystem.CurrentRenderMode =EvolutionSystem::RenderMode::POINT;
                    break;
                default:
                    break;
            }
            break;
        }
    }
}
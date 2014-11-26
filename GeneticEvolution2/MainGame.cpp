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
    ConfigurationManager configManager(resourcePath() + "configuration.txt");
    
    //if SDL fails, close program
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL!  " + std::string(SDL_GetError()));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    if (configManager.GetItem<bool>("Multisampling"))
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, configManager.GetItem<int>("MultisampleBuffers"));
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, configManager.GetItem<int>("MultisampleSamples"));
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    window = SDL_CreateWindow("Genetic Algorithm", 0, 0, configManager.GetItem<float>("WindowWidth"), configManager.GetItem<float>("WindowHeight"), SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    if (window==nullptr) throw std::logic_error("Window failed to be initialized");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("SDL_GL could not be initialized!");
    
    
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl", configManager);
    std::string fileLoc =resourcePath() + "performance.csv";
    EvolutionSystem evolutionSystem(fileLoc, configManager);
    Camera camera(configManager.GetItem<float>("WindowWidth"), configManager.GetItem<float>("WindowHeight"), configManager);
    
    while (GameState!=GameState::EXIT)
    {
        Update(evolutionSystem);
        camera.Update();
        glManager.Programs[0].SetMatrix4("transformMatrix", glm::value_ptr(camera.GetTransformMatrix()));
        Draw(evolutionSystem);
        SDL_GL_SwapWindow(window);
        HandleEvents(evolutionSystem,camera);
    }
}

void MainGame::Update(EvolutionSystem& evolutionSystem)
{
    evolutionSystem.Update();
}

void MainGame::Draw(EvolutionSystem& evolutionSystem)
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    evolutionSystem.Draw();
}

void MainGame::HandleEvents(EvolutionSystem& evolutionSystem, Camera& camera)
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
                    evolutionSystem.NextRenderMode();
                    break;
                case SDL_SCANCODE_SPACE:
                    evolutionSystem.ToggleAccelerated();
                    break;
                case SDL_SCANCODE_UP:
                    evolutionSystem.NextSelectedAgent();
                    break;
                case SDL_SCANCODE_DOWN:
                    evolutionSystem.PrevSelectedAgent();
                    break;
                case SDL_SCANCODE_T:
                    evolutionSystem.IncreasePlaybackRate();
                    break;
                case SDL_SCANCODE_G:
                    evolutionSystem.DecreasePlaybackRate();
                    break;
                default:
                    break;
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
                evolutionSystem.SelectClosestAgent(camera.GetPosition());
                break;
        }
        
    }
}
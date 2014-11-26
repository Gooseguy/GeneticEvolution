//
//  MainGame.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/12/14.
//  Copyright (c) 2014 Test. All rights reserved.
//
#pragma once
#include <SDL2/SDL.h>
#include "EvolutionSystem.h"
#include <unistd.h>
#include <string>

class Camera;
class MainGame
{
public:
    enum class GameState
    {
        OPEN, EXIT
    } GameState;
    MainGame();
    void Update(EvolutionSystem& evolutionSystem);
    void Draw(EvolutionSystem& evolutionSystem);
    void HandleEvents(EvolutionSystem& evolutionSystem, Camera& camera);
private:
    SDL_Window* window;
};
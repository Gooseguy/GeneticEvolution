//
//  RandomUtils.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/17/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once

#include <random>

class RandomUtils
{
public:
    static RandomUtils Instance;
    template<typename T>
    T Normal(T mean, T variance);
    template<typename T>
    T Binomial(int mean, float variance); // This is the discrete analog of the normal distribution
    template<typename T>
    T Uniform(T min, T max);
    
    std::mt19937 rand;
    float UniformFloat();
private:
};


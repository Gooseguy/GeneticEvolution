//
//  RandomUtils.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/17/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "RandomUtils.h"

RandomUtils RandomUtils::Instance;

template<typename T>
T RandomUtils::Normal(T mean, T variance)
{
    std::normal_distribution<T> distribution(mean, variance);
    return distribution(rand);
}
template<typename T>
T RandomUtils::Binomial(int mean, float variance)
{
    std::binomial_distribution<T> distribution(mean, variance);
    return distribution(rand);
}
template<typename T>
T RandomUtils::Uniform(T min, T max)
{
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(rand);
}
float RandomUtils::UniformFloat()
{
    return Normal<float>(0.0f,1.0f);
}
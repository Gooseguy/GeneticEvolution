//
//  ConfigurationManager.h
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/24/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//
#pragma once
#include <unordered_map>
#include <string>

class ConfigurationManager
{
public:
    ConfigurationManager(const std::string& fileName);
    template<typename T> T GetItem(const std::string& name);
    template<typename T> void SetItem(const std::string& name, T value);
private:
    std::unordered_map<std::string, float> floatItems;
    std::unordered_map<std::string, int> intItems;
    const char SEPARATOR_CHAR;
};

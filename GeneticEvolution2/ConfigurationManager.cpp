//
//  ConfigurationManager.cpp
//  GeneticEvolution
//
//  Created by Christian Cosgrove on 11/24/14.
//  Copyright (c) 2014 Christian. All rights reserved.
//

#include "ConfigurationManager.h"
#include <fstream>

ConfigurationManager::ConfigurationManager(const std::string& fileName) : SEPARATOR_CHAR(':')
{
    std::ifstream stream(fileName);
    if (!stream.is_open()) throw std::logic_error("Failed to open configuration file.");
    std::string line;
    while (std::getline(stream, line))
    {
        auto charPos = line.find(SEPARATOR_CHAR);
        std::string name = line.substr(2, charPos-2);
        std::string valueStr = line.substr(charPos+1);
        char itemType = line[0];
        switch (itemType)
        {
            case 'F':
                floatItems[name] = std::atof(valueStr.c_str());
                break;
            case 'I':
                intItems[name] = std::atoi(valueStr.c_str());
                break;
        }
        
    }
}


template<>
float ConfigurationManager::GetItem<float>(const std::string& name)
{
    if (floatItems.find(name)==floatItems.end()) throw std::out_of_range("Item not part of configuration.");
    return floatItems[name];
}

template<>
void ConfigurationManager::SetItem<float>(const std::string& name, float value)
{
    floatItems[name]=value;
}


template<>
int ConfigurationManager::GetItem<int>(const std::string& name)
{
    if (intItems.find(name)==intItems.end()) throw std::out_of_range("Item not part of configuration.");
    return intItems[name];
}

template<>
void ConfigurationManager::SetItem<int>(const std::string& name, int value)
{
    intItems[name]=value;
}
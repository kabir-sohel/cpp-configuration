//
//  ConfigurationEvent.cpp
//  Configuration
//
//  Created by Kabir Sohel on 12/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#include "ConfigurationEvent.h"

  








ConfigurationEvent::ConfigurationEvent(EventType eventType,string fileName, string key, string value)
{
    this->eventType = eventType;
    this->fileName = fileName;
    this->key = key;
    this->value = value;
}

std::string ConfigurationEvent::getKey()
{
    return this->key;
}
std::string ConfigurationEvent::getFileName()
{
    return this->fileName;
}
std::string ConfigurationEvent::getValue()
{
    return this->value;
}

EventType ConfigurationEvent::getEventType()
{
    return this->eventType;
}
//
//  ConfigurationException.h
//  Configuration
//
//  Created by Kabir Sohel on 14/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#ifndef __Configuration__ConfigurationException__
#define __Configuration__ConfigurationException__

#include "CommonHeader.h"


class ConfigurationException
{
public:
    std::string message;
public:
    ConfigurationException(std::string message)
    {
        this->message = message;
    }
    std::string getMessage()
    {
        return this->message;
    }
};








#endif /* defined(__Configuration__ConfigurationException__) */

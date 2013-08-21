//
//  ConfigurationEvent.h
//  Configuration
//
//  Created by Kabir Sohel on 12/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#ifndef __Configuration__ConfigurationEvent__
#define __Configuration__ConfigurationEvent__

#include "CommonHeader.h"








//enums



typedef enum
{
    CONFIGURATION_ADDED,
    CONFIGURATION_DELETED,
    CONFIGURATION_MODIFIED,
    
}EventType;









//classes


class ConfigurationEvent
{
private:
    EventType eventType;
    std::string key; //optional
    
    std::string value; //optional
    std::string fileName;
public:
    ConfigurationEvent(EventType eventType, string fileName, string key = "", string value = "");
    
    std::string getKey();
    std::string getValue();
    EventType getEventType();
    std::string getFileName();
};




//interfaces

class ConfigurationEventListener {
    
public:
    
protected:
public:
    virtual void configurationChanged(ConfigurationEvent event) = 0;
    virtual ~ConfigurationEventListener(){}
};



#endif /* defined(__Configuration__ConfigurationEvent__) */


//
//  ConfigurationMgr.cpp
//  Configuration
//
//  Created by Kabir Sohel on 12/9/12.
//  Copyright (c) 2012 Bubble Motion. All rights reserved.
//

#include "ConfigurationMgr.h"



ConfigurationMgr::ConfigurationMgr()
{
    fileMonitorThreadId = 0;
    fileMonitorThread = NULL;
}
ConfigurationMgr::~ConfigurationMgr()
{
    delete instance;
}

ConfigurationMgr* ConfigurationMgr::Instance()
{
    if(NULL == instance)
    {
        instance = new ConfigurationMgr();
    }
    return instance;
}




ConfigurationMgr* ConfigurationMgr::instance = NULL;




void ConfigurationMgr::startConfigurationMgr()
{
    fileMonitor = FileMonitor::getInstance();
    fileMonitor->addListener(this);
    
    fileMonitorThreadId = pthread_create(&fileMonitorThread, NULL, FileMonitor::fileMonitorLoop, NULL);
}





void ConfigurationMgr::loadXMLConfig(const string &fileName)
{
    //to be implemented later
}

void ConfigurationMgr::loadPropertyConfig(const string &fileName)
{
    
    
    if("" == fileName)
    {
        throw new ConfigurationException("Invalid file Name");
    }
    std::ifstream ifs(fileName.c_str());
    
    if(!ifs.is_open())
    {
        //cout << "Exception inside\n";
        string msg = "File : " + fileName + "opening Error for reading";
        
        //ConfigurationException exception(msg);
        throw new ConfigurationException(msg);     //implement every possible type of file exception, at future.

    }
    else
    {
        //std::cout << "Configuration Data is being Loaded from file " << fileName << std::endl;
        std::vector<std::string> data;
        std::string line;
        while(getline(ifs , line))
        {
            data.push_back(line);
        }
        
        //increase the fileNumber
        
        parseRawInput(data,fileName);
    }
}




void ConfigurationMgr::parseRawInput(std::vector<std::string> &data, const string &fileName)
{
    //parse based on the delimiter '='
    
    
    
    for(int i = 0; i < data.size(); ++i)
    {
        std::string key = "";
        std::string value = "";
        bool equalFound = false;
        
        string &line = data[i];
        
        Utility::removeComment(line);
        Utility::removeLeadingAndTrailingSpaces(line);
        
        if("" == line)continue;
        
        
        for(int j = 0; j < data[i].size(); ++j)
        {
            if('=' == line[j])
            {
                equalFound = true;
                continue;
            }
            if(true == equalFound)
            {
                value.push_back(line[j]);
            }
            else
            {
                key.push_back(line[j]);
            }
        }
        if(false == equalFound)
        {
            cout << "Invalid Property is being found, being ignored\n";
            continue;
        }
        
        Utility::removeLeadingAndTrailingSpaces(key);
        Utility::removeLeadingAndTrailingSpaces(value);
        addProperty(fileName, key, value);
        //cout << key << value << endl;
    }
}



void ConfigurationMgr::addPropertyFile(const string &fileName)
{
    addSource(fileName);   //configuration file
    fileMonitor->addFile(fileName);     //fileMonitor file
}


void ConfigurationMgr::addPropertyFile(int numberOfFiles, ...)
{
    va_list fileNames;
    va_start(fileNames, numberOfFiles);
    string msg = "";
    bool exceptionOccurred = false;
    for(int i = 0; i < numberOfFiles; ++i)
    {
        string fileName = va_arg(fileNames, char *);
        try{
            addPropertyFile(fileName);
        }
        catch (ConfigurationException *exception)
        {
            exceptionOccurred = true;
            msg += exception->getMessage();
        }
    }
    if(true == exceptionOccurred)
    {
        //ConfigurationException exception(msg);
        throw new ConfigurationException(msg);
    }
    
}


void ConfigurationMgr::removePropertyFile(const string &fileName)
{
    removeSource(fileName);
    fileMonitor->stopMonitoring(fileName);
}


